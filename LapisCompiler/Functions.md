//the pattern for a function is:

[OPTIONAL: access modifier] [OPTIONAL: function type] function [name]([OPTIONAL: variables]) -> [return type] {
  //some weird code here
}

//for example(no keywords):

function Add(a,b) -> int{
	return a + b;
}

//a single keyword:

async function Add(a,b) -> int{
	return a + b;
} 

//two keywords (e.g, with an access modifier):

public static function Add(a,b) -> int{
	return a + b;
}

//a function can also return many objects or have many return types:

function Add(a,b) -> int || string{
	//either int or a string will be returned
}

function Add(a,b) -> (int, int , int){ //strict rules
	return {
		a,
		b,
		a+b
	};
}

function Add(a,b) -> multiple{ //tells the compiler to figure everything out by itself
	return {
		a,
		b,
		a+b
	};
}

//SPECIAL FUNCTION TYPES:

//"Object" function -> a function that returns an object of any type,
//lets the compiler figure out the return type on the fly:

function Add(a,b) -> Object(){ //could be anything, a struct, class, type and etc...
	let s1 : Socket = new Socket(ID: 1, IP: "127.0.0.0", ....);
	return s1; //the compiler now knows that the returned object is a Socket()
}

//"radioactive" function -> a macro function.
//during runtime. for example:

radioactive function IncreaseVal(a){
	return a + 1;
}

IncreaseVal(1); //returns 2

//disposable functions -> functions that can be used only once, and deleted right after

disposable function Add(a,b) -> int{
	return a + b;
}
Add(2,2); //returns 4 
Add(2,2); //compile time error -> object already disposed

//function with optional values:

function Add(a = 1, b = 2){
	return a+b;
}
Add(1,4); //5
Add(); //3
Add(b: 3); //specified that the passed value is variable b's value. returns 4

//"unfinished" function -> tells the compiler to stop being annoying 
//and ignores the unimplemented function completely:

function IAmLazyToWriteTheFunctionButIWillPushItToProd() -> string{
	unfinished;
}



