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

let variable = function DoSomething(x) -> int {
	...
}
variable(number)
//as shown in the example above, variable isnt bounded to a single type, but can be a function

//if the type of the variable wasnt specified, the compiler will choose it independently



Function Declarations:
let DoSomething be a function that takes an argument, and returns a number.
it's signature will look like:

function DoSomething(variable) -> int{

}
//the keyword "function" specifies that DoSomething is a function, then, comes the name, 
//then the variables, and then "->" which points towards the desired return type, allowing:

function DoSomething(variable) -> int || string{

}
//now, DoSomething can return an int, or a string. the programmer will have to check
//the returned type to avoid any exceptions down the way
//the keyword function can be paired with other keywords:
//let keyword1 and keyword2 be keywords in Lapis:
{keyword1} {keyword2} fucntion DoSomething(variable) -> int || string{

}
//keyword1 can be any keyword with a higher priority than keyword2 (e.g, keyword1
is the access-modifier), including:
// public, private, protected, internal
//while keyword2 includes sub-priority keywords like:
//static, abstract, virtual, async, override, unsafe, partial


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
