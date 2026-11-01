//the pattern for declaration is:

{keyword} {variable name} {OPTIONAL: type specifier} {OPTIONAL: value setter} {OPTIONAL: some value}

//for example (without the type specifier):

let variable;

//with type specifier:

let variable : string;

//initialization with a value

let variable : int = 5;
let variable = 5; //for more context, see the notes section at the bottom
//or (const)
const variable : int = 5;
const variable = 5;
//initialization with an object:

let functionVariable = function(a,b) -> int{
	return a + b;
}
let variable = functionVariable;
let result = variable(3,4) //will return 7 and put it inside result

//NOTES:
//an object can be anything -> class, struct, function, type and so on
//if the type wasn't specified, the compiler will figure it out
//variable's type should match the value's type, of course

