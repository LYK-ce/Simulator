// #pragma once

// #include <iostream>
// #include <functional>
// #include <map>
// #include <optional>
// #include <utility>
// #include <cassert>

// namespace fsm {

// // Forward declaration of the StateMachine
// template<typename TState, typename TEvent, typename TContext>
// class StateMachine;

// /**
//  * @brief A builder for creating a StateMachine instance.
//  *
//  * This class provides a fluent API to define states, transitions, actions, and guards.
//  * It ensures that the StateMachine is constructed in a valid and immutable state.
//  *
//  * @tparam TState The type for states (usually an enum class).
//  * @tparam TEvent The type for events (usually an enum class).
//  * @tparam TContext The type for the context object (a struct or class holding shared data).
//  */
// template<typename TState, typename TEvent, typename TContext>
// class Builder {
// public:
//     using StateType = TState;
//     using EventType = TEvent;
//     using ContextType = TContext;
//     using GuardType = std::function<bool(const ContextType&)>;
//     using ActionType = std::function<void(ContextType&)>;

// private:
//     // A transition bundles the target state and optional guard/action
//     struct Transition {
//         StateType to_state;
//         std::optional<GuardType> guard;
//         std::optional<ActionType> action;
//     };

//     // The key for our transition table is the combination of a source state and an event
//     using TransitionKey = std::pair<StateType, EventType>;
    
//     // Custom comparator for std::map key
//     struct TransitionKeyCompare {
//         bool operator()(const TransitionKey& a, const TransitionKey& b) const {
//             if (a.first < b.first) return true;
//             if (b.first < a.first) return false;
//             return a.second < b.second;
//         }
//     };

//     using TransitionTable = std::map<TransitionKey, Transition, TransitionKeyCompare>;
//     using StateActionTable = std::map<StateType, ActionType>;

//     TransitionTable transitions_;
//     StateActionTable on_enter_actions_;
//     StateActionTable on_exit_actions_;
//     ContextType& context_; // Keep a reference to the user's context object

// public:
//     explicit Builder(ContextType& context) : context_(context) {}

//     /**
//      * @brief Defines an "on enter" action for a specific state.
//      * @param state The state to which the action is attached.
//      * @param action The function to execute upon entering the state.
//      */
//     Builder& on_enter(StateType state, ActionType action) {
//         on_enter_actions_[state] = std::move(action);
//         return *this;
//     }

//     /**
//      * @brief Defines an "on exit" action for a specific state.
//      * @param state The state to which the action is attached.
//      * @param action The function to execute upon exiting the state.
//      */
//     Builder& on_exit(StateType state, ActionType action) {
//         on_exit_actions_[state] = std::move(action);
//         return *this;
//     }

//     /**
//      * @brief Proxy class to chain guard and action definitions for a transition.
//      */
//     class TransitionBuilder {
//     private:
//         Transition& transition_;
//         Builder& parent_builder_;
//     public:
//         TransitionBuilder(Transition& transition, Builder& parent) 
//             : transition_(transition), parent_builder_(parent) {}

//         TransitionBuilder& with_guard(GuardType guard) {
//             transition_.guard = std::move(guard);
//             return *this;
//         }

//         TransitionBuilder& with_action(ActionType action) {
//             transition_.action = std::move(action);
//             return *this;
//         }

//         // Allow chaining back to the main builder for other definitions
//         Builder& then() {
//             return parent_builder_;
//         }
//     };

//     /**
//      * @brief Defines a transition from a source state to a target state, triggered by an event.
//      * @return A TransitionBuilder to optionally add a guard or action.
//      */
//     [[nodiscard("Chain with_guard, with_action, or then")]]
//     TransitionBuilder add_transition(StateType from, EventType event, StateType to) {
//         TransitionKey key = {from, event};
//         assert(transitions_.find(key) == transitions_.end() && "Transition already defined!");
        
//         Transition transition{to, std::nullopt, std::nullopt};
//         auto [it, success] = transitions_.insert({key, transition});
        
//         return TransitionBuilder(it->second, *this);
//     }

//     /**
//      * @brief Constructs the final StateMachine.
//      * @param initial_state The state the machine will start in.
//      * @return A fully configured StateMachine instance.
//      */
//     [[nodiscard]]
//     StateMachine<TState, TEvent, TContext> build(StateType initial_state) {
//         // The StateMachine's constructor is private, so we use `friend` to allow the builder to call it.
//         return StateMachine<TState, TEvent, TContext>(
//             initial_state,
//             context_,
//             std::move(transitions_),
//             std::move(on_enter_actions_),
//             std::move(on_exit_actions_)
//         );
//     }
// };


// /**
//  * @brief A generic, context-aware Finite State Machine.
//  *
//  * This class manages the current state and processes events to trigger transitions.
//  * It should be constructed via the `fsm::Builder`.
//  *
//  * @tparam TState The type for states (usually an enum class).
//  * @tparam TEvent The type for events (usually an enum class).
//  * @tparam TContext The type for the context object.
//  */
// template<typename TState, typename TEvent, typename TContext>
// class StateMachine {
// public:
//     using StateType = TState;
//     using EventType = TEvent;
//     using ContextType = TContext;
    
//     // Alias internal types from the Builder for convenience
//     using Transition = typename Builder<TState, TEvent, TContext>::Transition;
//     using TransitionTable = typename Builder<TState, TEvent, TContext>::TransitionTable;
//     using StateActionTable = typename Builder<TState, TEvent, TContext>::StateActionTable;
//     using TransitionKey = typename Builder<TState, TEvent, TContext>::TransitionKey;

// private:
//     friend class Builder<TState, TEvent, TContext>;

//     StateType current_state_;
//     ContextType& context_; // Reference to the shared data
//     const TransitionTable transitions_;
//     const StateActionTable on_enter_actions_;
//     const StateActionTable on_exit_actions_;

//     // Private constructor: can only be created by the Builder
//     StateMachine(
//         StateType initial_state,
//         ContextType& context,
//         TransitionTable transitions,
//         StateActionTable on_enter_actions,
//         StateActionTable on_exit_actions
//     )
//         : current_state_(initial_state),
//           context_(context),
//           transitions_(std::move(transitions)),
//           on_enter_actions_(std::move(on_enter_actions)),
//           on_exit_actions_(std::move(on_exit_actions))
//     {
//          // Execute the on_enter action for the initial state
//         auto it = on_enter_actions_.find(current_state_);
//         if (it != on_enter_actions_.end()) {
//             it->second(context_);
//         }
//     }

// public:
//     // StateMachines are stateful and shouldn't be copied.
//     StateMachine(const StateMachine&) = delete;
//     StateMachine& operator=(const StateMachine&) = delete;
//     // But they can be moved.
//     StateMachine(StateMachine&&) = default;
//     StateMachine& operator=(StateMachine&&) = default;

//     /**
//      * @brief Processes an event, potentially causing a state transition.
//      * @param event The event to process.
//      * @return True if a state transition occurred, false otherwise.
//      */
//     bool process_event(EventType event) {
//         const TransitionKey key = {current_state_, event};
//         auto transition_it = transitions_.find(key);

//         // 1. Event is not handled in the current state
//         if (transition_it == transitions_.end()) {
//             return false;
//         }

//         const auto& transition = transition_it->second;

//         // 2. Check guard condition
//         if (transition.guard && !(*transition.guard)(context_)) {
//             return false; // Guard failed, no transition
//         }

//         const auto target_state = transition.to_state;

//         // An internal transition does not change state, so no exit/enter actions are called.
//         if (target_state == current_state_) {
//             if (transition.action) {
//                 (*transition.action)(context_);
//             }
//             return false; // Not a state *change*, but action was potentially executed.
//         }

//         // 3. Execute On-Exit action for the current state
//         auto exit_it = on_exit_actions_.find(current_state_);
//         if (exit_it != on_exit_actions_.end()) {
//             exit_it->second(context_);
//         }

//         // 4. Execute transition action
//         if (transition.action) {
//             (*transition.action)(context_);
//         }

//         // 5. Change state
//         current_state_ = target_state;

//         // 6. Execute On-Enter action for the new state
//         auto enter_it = on_enter_actions_.find(current_state_);
//         if (enter_it != on_enter_actions_.end()) {
//             enter_it->second(context_);
//         }

//         return true;
//     }

//     /**
//      * @brief Gets the current state of the machine.
//      */
//     [[nodiscard]]
//     StateType current_state() const {
//         return current_state_;
//     }
    
//     /**
//      * @brief Gets a const reference to the context object.
//      */
//     [[nodiscard]]
//     const ContextType& context() const {
//         return context_;
//     }
// };

// } // namespace fsm
