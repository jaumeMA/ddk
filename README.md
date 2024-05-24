DDK Library
===================

The DDK library covers the following topics:

* **Iterability**: The DDK library offers a set of classes capable to offering iterable visitation based on actions. This infrastructure offers:
  1. Iterable adaptors for visitation, transformation, filtering, ordering, as can be seen in the following example:

  ```
  [&](const int& i_value)
  {
  	value++;
  } <<= ddk::view::order(ddk::reverse_order) <<= ddk::view::filter([](const int& i_value) { return i_value > 0; }) <<= ddk::iter::transform_as<int>([](auto&& i_value) {
  return 2 * i_value; }) <<= v_prova;
  ```

  2. Composition of iterables offering other iterables such as:

  ```
  [](const A& i_value)
  {
  	int a = 0;
	  ++a;
  } <<= ddk::iter::transform([](const A& i_val1,const D& i_val2,const E& i_val3) -> A { return i_val1 + i_val2 + i_val3; })
    <<= ddk::view::filter([](const A& i_val1,const D& i_val2,const E& i_val3) { return i_val1 > i_val2; })
    <<= ddk::view::order(ddk::reverse_order)
    <<= ddk::fusion(container1,container2,container3);
  ```

  3. Iterable actions for algorithms:

  ```
  	ddk::swap_action{} >>= ddk::iter::transform([](std::pair<const int,int>& i_value) -> int& { return i_value.second; })
				<<= ddk::view::filter([](const std::pair<const int,int>& i_value) { return i_value.second > 0; }) 
				<<= _foo;
  ```

	Iterable actions act as building blocks and allowing to create action graphs for algorithm definition.
	
  4. Agnostic iterables for api implementation:

  ```
  	ddk::const_bidirectional_value_iterable<const int> _ = ddk::iter::transform([](const std::pair<const int,int>& i_value) -> const int& { return i_value.second; }) 
								<<= ddk::view::filter([](const std::pair<const int,int>& i_value) { return i_value.second > 0; }) 
  															<<= _foo;
  ```
	
   This feature allows to offer agnostic from container iterables. The client code has the hability to select allocator for allocatin the resulting iterable.
			
* **Async operations**: The DDK library offers a suite of classes (typically promises, futures, continuations...) for executing operations in an async fashion.
	This infrastructure allows:

  1. Execution context selection. While creating a new async operation one can attach this operation to a certain execution context:

  ```
  ddk::async([&]()
  { 
  	return ddk::make_unique_reference<int>(10); 
  }) -> attach(std::move(_myThread))
  ```
	
  2. Custom promises. In case it is needed to store result of async operation in a customized promise (for specifying its allocator for instance) one can:

  ```
  ddk::future<void> provaFuture = ddk::async([&]() 
  {
  	recursive_func();
  }) -> store<MyPromise>();
  ```
	
  3. Custom scheduler. By default deferred scheduler is selected if no execution context is provided, or asap scheduler if it is, but still custom schedulers are allowed:

  ```
  ddk::future<int&> res = ddk::async([counter = 0]() mutable 
  { 
  	return counter++;
  }) -> schedule<ddk::polling_async_scheduler>()
  ```
	
  4. Custom allocators are allowed:

  ```
  ddk::future<char> myFuture = ddk::async<ddk::system_extack_allocator>(ddk::make_function([]() { return 'a'; }),local_allocator(1024));
  ```
  	
    5. Continuations are supported:
  
  ```
  ddk::future<char> myFuture = ddk::async(ddk::make_function([]() { return 'a'; })) -> attach(std::move(_myThread))
  .then([](char i_value)
  {
  	printf("valor rebut %c\n",i_value);
  
  	return i_value;
  });
  ```
	
  6. Future compositions are supported:

  ```
  ddk::future<int> myComposedFuture = compose(std::move(myCharFuture), std::move(myIntFuture))
  .then([](std::tuple<char,int> i_data)
  {
  	return std::get<1>(i_data);
  });
  
  compose(std::move(myOtherIntFuture),std::move(myComposedFuture))
  .then([](std::array<int, 2> i_data)
  {
  	return static_cast<float>(i_data[0] + i_data[1]);
  })
  .then([](float i_data)
  {
  	int a = static_cast<int>(i_data);
  	++a;
  });
  ```
	
  7. Higher order futures:

  ```
  ddk::async([&]() -> my_result
  { 
  	return ddk::make_unique_reference<int>(10); 
  }) -> attach(std::move(_myThread))
  .then([_myThread = std::move(myThread)](my_result i_value) mutable -> ddk::future<std::string>
  {
  	if (i_value)
  	{
  		return ddk::async([]() { return std::string("sucess"); }) -> attach(std::move(_myThread));
  	}
  	else
  	{
  		return ddk::async([]() { return std::string("error"); }) -> attach(std::move(_myThread));
  	}
  })
  .then([_myOtherThread = std::move(myOtherThread)](std::string i_value) mutable -> ddk::future<std::string>
  {
  	return ddk::async([i_value]() { return i_value + std::string(":sucess"); }) -> attach(std::move(_myOtherThread));
  })
  .then([](std::string i_value)
  {
  	return 10;
  });
  ```

* **General purpose utilities**:
	
	1. Smart pointers such as unique pointers/references, shared/distributed pointers/references, weak pointers, lent pointers/references.
	2. Generalized functions allowing higher order functions.
	3. Custom threads and fibers with the hability to create thread and fiber pools.
	4. Multithread related utilities such as multiple reader writer infrastructure, lock free queues, task executors, atomics...
	5. Optionals and Variants.
	6. General template metaprogramming infrastructure.
	7. And many more...
		
Installing CMake
----------------

In order to generate DDK project, you need CMake 3.13 or newer. You can download the latest version at this link:

https://cmake.org/download/

Just launch the installer and follow the instructions.

Installing Perl
---------------

In order to generate CMake scripts, you need Perl. You can download the latest version at ActiveState website:

https://www.activestate.com/products/perl/downloads/

Just launch the installer and follow the instructions.

Preconditions
=============

This code compiles in windows visual studio 2015 (15.7 or later) and in linux gcc 7 or later.

Building the Project
====================

- Clone the repository
- execute perl script genPrj-XXX.pl (where XXX stands for platform)

The **ddkXXX.sln** solution will be located in the `build` folder, while generated binaries will be generated in the `bin` folder.
Unfortunately, at some point this library stopped compiling in Linux and now only windows visual studio compiler (using 17 standard) is supported.
