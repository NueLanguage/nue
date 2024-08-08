// Define a complex function to analyze and categorize text based on various metrics
private var function analyzeText = function(string text) -> number, string {
    private var number length = #text
    private var string longOrShort

    if length > 100 {
        longOrShort = "Very Long"
    } else if length > 50 {
        longOrShort = "Long"
    } else {
        longOrShort = "Short"
    }

    return length, longOrShort
}

// Function to reverse a string
private var function reverseString = function(string original) -> string {
    private var string reversed = ""
    for i in range(1, #original + 1) {
        reversed = original[i] + reversed
    }
    return reversed
}

// Use table to store multiple types of data
private var table myTable = {
    name = "NuLang User",
    age = 30,
    interests = {"coding", "reading", "gaming"},
    description = function() -> string {
        return "A passionate coder who loves technology."
    }
}

// Print information about the user
print("User Profile:")
print("Name:", myTable.name)
print("Age:", myTable.age)
print("Interests:")
for interest in myTable.interests {
    print("-", interest)
}

// Adding more interests
myTable.interests += {"music", "traveling"}

// Accessing and using the description function
private var string userProfile = myTable.description()
print("Profile Description:", userProfile)

// Analyzing a text sample
private var string sampleText = "This is a sample text for NuLang, designed to showcase the language's capabilities and flexibility."
private var number textLength, string textCategory = analyzeText(sampleText)
print("Sample Text Analysis:")
print("Text Length:", textLength)
print("Category:", textCategory)

// Demonstrating text reversal
private var string originalText = "Hello, NuLang!"
private var string reversedText = reverseString(originalText)
print("Original Text:", originalText)
print("Reversed Text:", reversedText)

// Demonstrating control flow with conditions
if textLength > 50 & textCategory == "Long" {
    print("The text is quite lengthy and considered long.")
} else {
    print("The text is of moderate length.")
}

// Looping through table with index and value
print("Updated Interests:")
for index, value in myTable.interests {
    print(index, ":", value)
}

// Demonstrating deletion of a variable
print("Deleting an unnecessary variable...")
delete(reversedText)
print("Variable 'reversedText' should now be deleted.")

// Final output to demonstrate various data types and control flow
print("Final Output:")
for key, value in myTable {
    print(key, ":", value)
}

// End of program
