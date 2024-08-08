print("Hello world!")

private var string myInput = input("Hello! Please enter your name below:\n")
print(myInput)

private var string noNewLine = input("Enter your age: ")

private var string yesNewLine = input("Enter your age below:\n")

private var number myNumber = 5 + 5
myNumber = 5 - 4
myNumber = 5 * 5
myNumber = 5 / 5
myNumber = 5 ^ 2

print(myNumber)

private var number myOriginalValue = 5
private alias myAliasedValue = myOriginalValue
print(myOriginalValue)
print(myAliasedValue)

myOriginalValue = myOriginalValue ^ 2

print(myAliasedValue)

myAliasedValue = 9

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

private var table thisIsTechnicallyAnArrayButTheTypeIsATableInNuBecauseOnlyTableTypesExist = {
    "foo",
    "bar",
    "yes",
    "no",
    true,
    false,
    8,
    9.3202,
    any function(number arg1, [number, string, null] arg2) {
        if type(arg2) == "number" {
            return arg1 + arg2
        } else {
            return arg1, arg2
        }
    }
}

for value in thisIsTechnicallyAnArrayButTheTypeIsATableInNuBecauseOnlyTableTypesExist {
    print(value)
    print(type(value))
}

for index, value in thisIsTechnicallyAnArrayButTheTypeIsATableInNuBecauseOnlyTableTypesExist {
    print(index, value)
}

print(#thisIsTechnicallyAnArrayButTheTypeIsATableInNuBecauseOnlyTableTypesExist) // 9
private var number doneAmount = 1
while doneAmount != #thisIsTechnicallyAnArrayButTheTypeIsATableInNuBecauseOnlyTableTypesExist {
    private alias theThing = thisIsTechnicallyAnArrayButTheTypeIsATableInNuBecauseOnlyTableTypesExist
    print(theThing[doneAmount], type(theThing[doneAmount]))

    doneAmount += 1
}

private var function myFunction = string function(string thing) {
    print("It looks like you entered" + thing)

    return "He entered " + thing + "!"
}

myFunction = string function(number inputtedNumber) {
    return "Bob the builder" + inpittedNumber
}

private var function myBetterWorkingFunction = string function(number inputNum) {
    private var string converted = tostring(inputNum)
    return "Finally, I think I might have " + converted + " eggs!"
}

private var function proceduresDontExistSoThisIsAFunction = null function(string thingToPrint) {
    print(thingToPrint)
    print("Well, what more did you expect? It won't return a value.")
    print("But here's what you tried to print: " + thingToPrint)
}

private var function combineStrings = string function(string firstString, [string, null] secondString) {
    return firstString + tostring(secondString)
}

print(combineStrings("Hello", "World"))
print(combineStrings("Hello"))

private var function combineRequired = string function(string firstString, [string, number] secondString) {
    return firstString + tostring(secondString)
}

print(combineRequired("Hello", "World"))
print(combineRequired("Hello", 9))
print(combineRequired("Hello"))

private var [number, null, table, boolean] myEverything

myEverything = null
print(type(myEverything))

myEverything = 5 + 8
print(myEverything)
print(type(myEverything))

myEverything = false
print(myEverything)
print(type(myEverything))

myEverything = "This is totally a normal string! I'm harmless!"