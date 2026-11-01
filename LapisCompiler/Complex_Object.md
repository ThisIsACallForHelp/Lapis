a simple class's syntax is:

[access-modifier] class [the name of the class] [OPTIONAL: inheritance] [OPTIONAL: parent classes]{
	//if you want to make public/private/protected variables, type "public:" / "private:" or "protected:"
	//be default, if undeclared, everything is private. types MUST be specified
	
	constructor({variables}){
		//init the varaibles here
	}
	//methods
}

an example without inheritance:

private class Person{
	public: 
		age : int;
		name : string;
	private:
		socialSecurityNumber : string;
		personalPhoneNumber : string;
	protected:
		DNA_Information : DNA; //GET IT? BECAUSE CHILDREN INHERIT THE GENES?? HAHAHAHAHAHAHAHAHAHAHAHA

	constructor(inputAge : int, inputName : string, inputSSN : string, inputPhone : string, inputDNA : DNA){
		age = inputAge;
		name = inputName;
		socialSecurityNumber = inputSSN;
		personalPhoneNumber = inputPhone;
		DNA_Information = inputDNA;
	}
	//note: you can also just write this.age = age;. i didnt since the names of the 
	//input variables and the names of the properties arent the same
}

//complex classes with inheritance:

public class Child extends Mother, Father{
	//will include every property of Mother and Father if they arent private
	constructor() => Mother(), Father() { 
		//some additional code if the class has more vars
	}
}

//special keywords:

public class Socket{
	port : int; [nocopy] //tells the compiler to replace the value with a default one (e.g, string -> "", int -> 0 and so on)
	IP : string [readonly] //will be initialized like a normal variable. then -> act like a const
	//....
}

let s1 : Socket = new Socket(port: 8679, IP: "127.0.0.0", ...);
let s2 = s1; //s2.port will hold default(int), a.k.a -> 0. the programmer must handle the 
//fields that shouldnt be copied personally. instead of writing many lines, theres is a shortcut:

let s3 = s1 replace { port: 7385, IP: "173.135.75.34", ...}; 
//the replace keyword makes it so that specific fields can be altered during the initialization

//the same syntax goes to the structs 


//"types" -> the "type" keyword declares a special kind of type that has:
//many properties, like a class
//doesnt hold any function in it 
//for example:

let userType : Type = {
	id : string;
	age : int;
	name : string;
}

let newUser : userType; //newUser has now a custom type 


//an interface is basically used for abstraction. if inherited, the programmer must 
//implement the function, or at least declare it and use the "unfinished" keyword

public interface Animal{
	function MakeSound() -> string;
}

public class Dog extends Animal{
	public function MakeSound() -> string{
		return "bark bark";
	}
}

public class Cat extends Animal{
	public function Meow() -> string{
		return "i am going to invest and buy bitcoin";
	}
	//the interface's function wasnt implemented
	//so the compiler punches the developer with an exception
}

