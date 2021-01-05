// #pragma once
// 
// #include <vector>
// #include <functional>
// 
// //------------------------------------------------------------------------
// // ...ARGS -> "List Of" Args
// template <typename ...ARGS>
// class Delegate
// {
// public:
// 	using function_t = std::function<void( ARGS... )>;
// 	using c_callback_t = void ( * )( ARGS... );
// 
// 	struct sub_t // subscription_t 
// 	{
// 		void const* obj_id = nullptr;
// 		void const* func_id = nullptr;
// 		function_t callable;
// 
// 		inline bool operator==( sub_t const& other ) const { return ( obj_id == other.obj_id ) && ( func_id == other.func_id ); }
// 	};
// 
// public:
// 	//------------------------------------------------------------------------
// 	void subscribe( c_callback_t const& cb )
// 	{
// 		sub_t sub;
// 		sub.func_id = cb;  // cb = &cb = &&cb = *cb = **cb = ***cb = ...
// 		sub.callable = cb;
// 
// 		subscribe( sub );
// 	}
// 
// 	//------------------------------------------------------------------------
// 	void unsubscribe( c_callback_t const& cb )
// 	{
// 		sub_t sub;
// 		sub.func_id = cb;
// 
// 		unsubscribe( sub );
// 	}
// 
// 	//------------------------------------------------------------------------
// 	template <typename OBJ_TYPE>
// 	void subscribe_method( OBJ_TYPE* obj , void ( OBJ_TYPE::* mcb )( ARGS... ) );
// 	{
// 		sub_t sub;
// 		sub.obj_id = obj;
// 		sub.func_id = *( void const** ) &mcb;
// 
// 		// std::bind
// 		// capture list
// 		sub.callable = [ = ] ( ARGS ...args ) { ( obj->*mcb )( args... ); };
// 
// 		subscribe( sub );
// 	}
// 
// 	//------------------------------------------------------------------------
// 	template <typename OBJ_TYPE>
// 	void unsubscribe_method( OBJ_TYPE* obj , void ( OBJ_TYPE::* mcb )( ARGS... ) )
// 	{
// 		sub_t sub;
// 		sub.obj_id = obj;
// 		sub.func_id = *( void const** ) &mcb;
// 
// 		unsubscribe( sub );
// 	}
// 
// 	// void unsubscribe_object( void const* obj_ptr ); 
// 	// void unsubscribe_all(); 
// 
// 	//------------------------------------------------------------------------
// 	// int args, int args
// 	void invoke( ARGS const& ...args )
// 	{
// 		for ( sub_t& sub : m_subscriptions )
// 		{
// 			sub.callable( args... );
// 		}
// 	}
// 
// 	void operator() ( ARGS const& ...args ) { invoke( args... ); }
// 
// private:
// 	//------------------------------------------------------------------------
// 	void subscribe( sub_t const& sub )
// 	{
// 		m_subscriptions.push_back( sub );
// 	}
// 
// 	//------------------------------------------------------------------------
// 	void unsubscribe( sub_t const& sub )
// 	{
// 		for ( uint i = 0; i < m_subscriptions.size(); ++i )
// 		{
// 			if ( m_subscriptions[ i ] == sub )
// 			{
// 				m_subscriptions.erase( m_subscriptions.begin() + i );
// 				return;
// 			}
// 		}
// 	}
// 
// 	std::vector<sub_t> m_subscriptions;
// };
// 
// 
// #include <vector>
// #include <functional>
// 
// 
// //--------------------------------------------------------------------------------------------------------------------------------------------
// 
// // the ...ARGS may be 0 or more arguments of type `typename` - allowing us to use this
// // with any number of argument types
// 
// template <typename ...ARGS>
// class Delegate
// {
// public:
// 	using c_callback_t = void ( * )( ARGS... );            // signature of a standalone C function that works with this Delegate
// 	using function_t = std::function<void( ARGS... )>;   // callable object to store it
// 	struct sub_t
// 	{
// 		void const* obj_id;           // object I'm calling on
// 		void const* func_id;          // address of function I'm calling
// 		function_t callback;    // callable object o call above function/method
// 
// 		inline bool operator==( sub_t const& other ) const { return ( obj_id == other.obj_id ) && ( func_id == other.func_id ); }
// 	};
// 	// typedef the callback type to save on typing later
// 
// public:
// 	void subscribe( c_callback_t const& cb );
// 	void unsubscribe( c_callback_t const& cb );
// 
// 	void unsubscribe_all();
// 
// 	void invoke( ARGS& ...args ) const; // will call all subscribers
// 	void operator() ( ARGS& ...args ) const { return invoke( args... ); } // allows us to use this object as a functor
// 
// private:
// 	// moving this private as no one will use these directly
// 	void subscribe( sub_t const& func_to_call );
// 	void unsubscribe( sub_t const& func_to_call );
// 
// public:
// 	std::vector<sub_t> m_subscribers;
// };
// 
// //--------------------------------------------------------------------------------------------------------------------------------------------
// 
// template <typename ...ARGS>
// void Delegate<ARGS...>::subscribe( c_callback_t const& cb )
// {
// 	sub_t sub;
// 
// 	// give it identifying information
// 	sub.obj_id = nullptr;            // no object
// 	sub.func_id = ( void const* ) &cb; // function we're calling has a unique address
// 									 // (fun trivia - cb == &cb == &&cb == *cb == **cb == ***cb.  Function pointers get weird rules)
// 	sub.callback = cb;               // actually be able to call it!
// 
// 	subscribe( sub );                // call to our common subscribe
// }
// 
// template <typename ...ARGS>
// void Delegate<ARGS...>::unsubscribe( c_callback_t const& cb )
// {
// 	// create a sub with just the identifying information...
// 	sub_t sub;
// 	sub.obj_id = nullptr;
// 	sub.func_id = ( void const* ) &cb;
// 	// no need to copy the actually callback - we won't be using it
// 
// 	// remove it from the list
// 	unsubscribe( sub );
// }
// 
// template <typename ...ARGS>   // template for the class
// template <typename OBJ_TYPE>  // template for the method
// void Delegate<ARGS...>::subscribe_method( OBJ_TYPE* obj , void ( OBJ_TYPE::* mcb )( ARGS... ) )
// {
// 	sub_t sub;
// 	sub.obj_id = obj;             // easy, pointer is pointer
// 	sub.func_id = *( void** ) &mcb; // compiler is going to try to disuade you hard from using the memory address of the method
// 								  // reason being it is not safe to use due to how methods pointers can be implemented
// 								  // but as identifying information - it'll be fiiiiiine.  
// 
// 	// now, how to get this into a function.  Couple options...
// 	// 1. std::bind  (boo, boring!)
// 	// 2. LAMBDA!!!! (omg so easy)
// 
// 	// So lambda - I'm going to do a capture by value for my two
// 	// pointer types, and allow the arguments to be passed through.
// 	sub.callback = [ = ] ( ARGS ...args ) { ( obj->*mcb )( args... ); };
// 
// 	subscribe( sub );
// }