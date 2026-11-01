so, we are now at the very starting point of our compiler, and we will have to actually think about
the syntax. i have some suggestions:

Variable Declaration:

let variable = 7; //"let" is a keyword, variable is the name and the value comes right after it.
let varialbe : uint32 = 6767; //same here, but there is a specified type.
let variable = GetSubString("someString") || 5;


//in the example above, assume that GetSubString is a function that takes a value, and outputs a substring.
//the compiler will figure out what type to assign variable:
//if, for some reason, GetSubString returns an empty value (e.g, null, "" and etc.),
//then the compiler will abandon the result and assign it the next value (e.g, 5 -> int)

let variable = function -> int {
	...
}
variable(number)

//as shown in the example above, variable isnt bounded to a single type, but can be a function

//if the type of the variable wasnt specified, the compiler will choose it independently



Function Declarations:


//let DoSomething be a function that takes an argument, and returns a number.
//it's signature will look like:

function DoSomething(variable) -> int{
	//code goes here
}

//the keyword "function" specifies that DoSomething is a function, then, comes the name, 
//then the variables, and then "->" which points towards the desired return type, allowing:

function DoSomething(variable) -> int || string{
	//code goes here
}

//now, DoSomething can return an int, or a string. the programmer will have to check

//also, a function can return many values, or an object full of paramters:

function ReturnManyVals() -> multiple{
	//code goes here
	return {
		returnedValue1: value1,
		returnedValue2: value2,
		....
		returnedValueM: valueM
	};
}

//the multiple keyword tells the compiler that ReturnManyVals() 
//will return multiple values of any types. to get the values:

let {value1, value2, ...., valueN} = ReturnManyVals()
//of course, the amount of values returned shouldnt be less than the amount of variables created.
//meaning, if M is the amount of variables returned by ReturnManyVals(), and N is the amount of vars delcared:
//M <= N
//if the programmer wants to share a value between two variables, the syntax will be:

let {value1 : returnedValue1, value2 : returnedValue1, ..., valueN : returnedValueM} = ReturnManyVals()

//when the values are returned like in ReturnManyVals(), each value will have a tag and a value
//in order to distinguish between the values and make the code more readable. 
//in order to initialize a variable to a specific value, the programmer must specify:
//the variable's name
//the target val's tag

function ReturnAnObject() -> Object{
	//code goes here. assuming that the programmer made an Object named "obj", then, we return:
	return obj
}

//"Object" is a special type, that could be any type, whether its a class/struct/interface
//also, if the programmer's intent is to declare a special, one time use kind of an Object, the one 
//that could be disposed right after the program ends and was not delcared before as:
//class/interface/struct
//the programmer can write the next return statement:

function ReturnOneTimeUseObject() -> Object{
	//code goes here. assume that the programmer declared some values (no matter their type):
	//value1, value2, ... , valueN
	return new Object(){
		value1,
		value2,
		....,
		valueN
	}
}



//the returned type to avoid any exceptions down the way
//the keyword function can be paired with other keywords:
//let keyword1 and keyword2 be keywords in Lapis:

{keyword1} {keyword2} function DoSomething(variable) -> int || string{

}

//keyword1 can be any keyword with a higher priority than keyword2 (e.g, keyword1
is the access-modifier), including:
// public, private, protected, internal
//while keyword2 includes sub-priority keywords like:
//static, abstract, virtual, async, override, unsafe, partial

Lambda/Arrow functions 

Loops:

For loop:
//Lapis includes standart for loops:
for(let i = 0; i < 10; i++){
	...
}
//while loops:
let i : int = 0;
while(i == 0){
	...
}
//and until loops:
let i : int = 0;
do{
	...
}until( i < 10)
