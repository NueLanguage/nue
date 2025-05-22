// Welcome to nulang, a fucking weird programming language that I made.
// Comments are started with double forward slashes, like this:
// this is a comment!

// Multi line comments are like this, and will continue up until the closing point:

//#
this
is
a
multiline
comment
#//

// possible types are:
// null, boolean, number, string, function, table
// idk what other types there should be
// by the way, floats dont exist in nu. a number is a number.
// 3.00 is the same as 3 and both are numbers.

// print with the print(...) function
print("Hello world!")

// take input from the command line with input()
private var string myInput = input("Hello! Please enter your name below:\n") // input() accepts a string as an input which prints something before accepting an input.
print(myInput)
// unlike print() by itself, input() does NOT create a new line after printing the input. You have to manually specify new lines like I did there with \n.
// this is so that you can either accept input from new lines or on the same line. Heres an example:

private var string noNewLine = input("Enter your age: ")
// Enter your age: __ // it did not create a new line, and where the __ is, you enter your age

private var string yesNewLine = input("Enter your age below:\n")
// Enter your age below:
// __

// variables and arithmetic operations
// private indicates that the scope of the variable is limited to the block where it is declared.
private var number myNumber = 5 + 5
myNumber = 5 - 4
myNumber = 5 * 5
myNumber = 5 / 5
myNumber = 5 ^ 2

print(myNumber) // 25

// aliases
// aliases are an incredibly cool feature and also unique, as im pretty sure this would be the first programming language to create such a feature.
// aliases allow you to essentially create another variable whilst tracking the original value.
private var number myOriginalValue = 5
// aliases are incredibly simply. you write the following, and it will inherit the predefined types of the original variable:
private alias myAliasedValue = myOriginalValue
// now, myAliasedValue will essentially always have the same value as myOriginalValue. its a tracked variable at that point.
// aliases are unlike simply creating a new variable and assigning the value of another variable to it.
print(myOriginalValue) // 5
print(myAliasedValue) // 5

myOriginalValue = myOriginalValue ^ 2

print(myAliasedValue) // 25
// see! we did nothing to myAliasedValue itself but it still essentially tracked what myOriginalValue was and updated.

// aliases can only be read from. cannot be written to.
myAliasedValue = 9
// ERROR: Attempted to assign value to read-only alias


// Control flow
// Conditionals
if true {
    print("Yay!")
}

private var string myString = "Hello guys"

if myString == "hello guys" {
    print("it was all lowercase")
} else if myString == "Hello guys" {
    print("Correct!")
} else {
    print("Something has gone catastrophically wrong...")
}

if 5 < 10 {
    print("5 is less than 10")
} else {
    print("Something catastrophic happened!")
}

if ((5 * 5 * 5 * 5 * 5) ^ 3) < (2 ^ 16) {
    print("Something is wrong.")
} else {
    print("30517578125 is bigger than 65536")
}

// tables
// tables are a powerful data structure in nue
private var table technicallyADictionaryButItsATable = {
    hello = "world",
    bye = "planet",
    lol = "rofl"
}

for index, value in technicallyADictionaryButItsATable {
    print(index, value)
}
// hello, world
// bye, planet
// lol, rofl

private var table moreStringsBruh = {
    hello = "bye",
    bye = "hello",
    ["laughing"] = "crying",
    ["string"] = "yep, if you cant write a index regularly because it conflicts with nue keywords then you can make the index inside a ssquared string (whatever its called)",
    ["愛してます"] = "and if there are unusual characters, then you can also put them into a string inside squares",
    ["😎"] = "this too!",
}

for index, value in moreStringsBruh {
    print(index, value)
}
// hello, bye
// bye, hello
// laughing, crying
// string, yep, if you cant write a index regularly because it conflicts with nue keywords then you can make the index inside a ssquared string (whatever its called)
// 愛してます, and if there are unusual characters, then you can also put them into a string inside squares
// 😎, this too!

private var table allMuddledUp = {
    "so",
    "far",
    "this",
    "seems",
    "like",
    "an",
    "array",
    "right?",
    but = "then",
    we = "can",
    give = "indexes",
    ["and"] = "it",
    becomes = "weird.",
    {
        {
            "im a nested table lol. you can literlally put anything inside tables basically and then nest them a trillion times if you want."
        }
    }
}

for index, value in allMuddledUp {
    print(index, value)
}
// 1, so
// 2, far
// 3, this
// 4, seems
// 5, like
// 6, an
// 7, array
// 8, right?
// but, then
// we, can
// give, indexes
// and, it
// becomes, weird.
// 9, {{"im a nested table lol. you can literally put anything inside tables basically and then nest them a trillion times if you want."}}

// trying to print a table will result in it printing the table. not in json format, but nue format. similar to what python does when you try to print a table.

// Loops
private var table thisIsTechnicallyAnArrayButTheTypeIsATableInNuBecauseOnlyTableTypesExist = {
    "foo",
    "bar",
    "yes",
    "no",
    true,
    false,
    8,
    9.3202,
    any function(number arg1, [number, string, null] arg2) { // functions cannot be declared private when inside a table because their scope is limited to the table regardless
        if type(arg2) == "number" {
            return arg1 + arg2
        } else {
            return arg1, arg2
        }
    }
}

for value in thisIsTechnicallyAnArrayButTheTypeIsATableInNuBecauseOnlyTableTypesExist { // if only one variable is declared then the for loop will assume that you only want the value of each key that we loop over. if you want the index, provide two variables. also these variables are private to and limited to the scope of the for loop.
    print(value)
    print(type(value))
}

//# OUTPUT OF THE FOR LOOP IN TERMINAL
foo
string
bar
string
yes
string
no
string
true
boolean
false
boolean
8
number
9.3202
number
function 9(number arg1, [number, string, null] arg2) --> any, 0x0002384758783
function
#//

for index, value in thisIsTechnicallyAnArrayButTheTypeIsATableInNuBecauseOnlyTableTypesExist { // when there are two variables in the for loop, for example index and value, it will assume that the first value will always be the index (or key as they call it in other programming languages), and the second variable will be the value of the index.
    print(index, value)
}
// also, tables start with 1 as the first index, unlike many other programming languages where the index always starts at 0.
//# OUTPUT IN TERMINAL
1, foo
2, bar
3, yes
4, no
5, true
6, false
7, 8
8, 9.3202
9, function 9(number arg1, [number, string, null] arg2) --> any, 0x0002384758783
#//

// when printing a function, NOT THE OUTPUTS OF A FUNCTION, but printing a function...:
// it will output the name of the function, arguments to be provided, and then the output type and finally the memory location of the function (i think) since we cant really provide any more info
// so essentially here is the format for a function being printed:
/// function nameOfFunction(type argumentName, ...) --> type, memoryLocation
// note that in this case, despite us not explicitly stating a function name, it assumed that the function name was simply 9 because that is the position of the function in the table. it is in ninth place, therefore the INDEX (name) is 9 and the function name is 9.

print(#thisIsTechnicallyAnArrayButTheTypeIsATableInNuBecauseOnlyTableTypesExist) // 9
private var number doneAmount = 1
while doneAmount != #thisIsTechnicallyAnArrayButTheTypeIsATableInNuBecauseOnlyTableTypesExist {
    private alias theThing = thisIsTechnicallyAnArrayButTheTypeIsATableInNuBecauseOnlyTableTypesExist // I LOVE ALIASES!! also this alias' scope is limited to this while block, meaning it cannot be referenced outside of the while loop.
    print(theThing[doneAmount], type(theThing[doneAmount]))

    doneAmount += 1
}

//#
foo, string
bar, string
yes, string
no, string
true, boolean
false, boolean
8, number
9.3202, number
function 9(number arg1, [number, string, null] arg2) --> any, 0x0002384758783, function
#//
// keep note that providing multiple arguments to print() will simply output the arguments separated by commas and a space.

// functions
// contrary to popular programming languages, functions cannot simply be defined just by writing "function functionName".
// in nue, functions are assigned to variables, and, in turn, the name of the variable becomes the name of the function.
// this also explains why when we had a function in the 9th position in the table, the function name was 9 when printed.
private var function myFunction = string function(string thing) {
    print("It looks like you entered" + thing)

    return "He entered " + thing + "!"
}

// remember that myFunction is a variable with the only assignable type being a function
// so we can assign this new function to myFunction
myFunction = string function(number inputtedNumber) {
    return "Bob the builder" + inpittedNumber // ERROR: Cannot concatenate string with number
    // yes, it wont automatically convert types in this instance. do it yourself.
}

private var function myBetterWorkingFunction = string function(number inputNum) {
    private var string converted = tostring(inputNum)
    return "Finally, I think I might have " + converted + " eggs!"
}

// likewise, a function may also not return a value.
// there is no difference between procedures and functions in nue.
// however, since this function returns nothing, the return type must be null or any
private var function proceduresDontExistSoThisIsAFunction = null function(string thingToPrint) {
    print(thingToPrint)
    print("Well, what more did you expect? It won't return a value.")
    print("But here's what you tried to print: " + thingToPrint)
}

// this is a function with multiple inputs
private var function combineStrings = string function(string firstString, [string, null] secondString) {
    return firstString + tostring(secondString)
    // performing tostring on null will result in "null"
}

print(combineStrings("Hello", "World")) // HelloWorld
print(combineStrings("Hello")) //Hellonull
// since the second argument allowed either string or null, we can simply omit the second argument from our call to the function and null will be provided automatically.

// if the second argument didn't accept null however and we didnt provide it, then we would receive an error:
// ERROR: Required argument #2 not provided to function combineStrings(string firstString, [string, number] secondString) --> string
// heres an example of this in action:
private var function combineRequired = string function(string firstString, [string, number] secondString) {
    return firstString + tostring(secondString)
    // performing tostring on a string will just return the string
    // performing tostring on a number will convert that number to a string yk
}

print(combineRequired("Hello", "World")) // HelloWorld
print(combineRequired("Hello", 9)) // Hello9
print(combineRequired("Hello"))
// ERROR: Required argument #2 not provided to function combineStrings(string firstString, [string, number] secondString) --> string

// variables with multiple types
private var [number, null, table, boolean] myEverything // you can initialise empty variables and assign a value to them later at any point!

myEverything = null
print(type(myEverything)) // null

myEverything = 5 + 8
print(myEverything) // 13
print(type(myEverything)) // number

myEverything = false
print(myEverything) // false
print(type(myEverything)) // boolean

myEverything = "This is totally a normal string! I'm harmless!"
// ERROR: Cannot assign string to variable accepting only types number, null, table, boolean.
// breaks