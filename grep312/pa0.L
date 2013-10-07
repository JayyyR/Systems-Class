(*CS 312 Programming Assignment 0*)
(*Timothy Cohen and Joseph Acosta*)
(*tacohen@email.wm.edu and jracosta@email.wm.edu*)


let _ = print "Please enter the number of assignments" in
let numAssignments = readInt in
let totalWeight = 0 in
let numStudents = 0 in
let listOfWeights = _ in
let studentAvg = _ in
let gradeList = _ in
let listOfAvg = _ in
let studentList = _ in
let totalNumber = 0 in
let tempListOfWeights = _ in
let gradeToAdd = 0 in
let sortedList = _ in
let templistOfAvg = _ in
let tempAvgToAdd = 0 in


(*Main function to grab weights from user*)
fun grabWeights with n =
if n=0 then 

if totalWeight <> 100 then
let _ = print "Your weights don't add up! Aborting..." in
1
else

(grabStudents numAssignments)

else
let _ = print "Please enter assignment weight" in
let assignWeight = readInt in

if assignWeight>100 then
let _ = print "Please enter a valid weight between 0 and 100" in
(grabWeights n)
else

if assignWeight<0 then
let _ = print "Please enter a valid weight between 0 and 100" in
(grabWeights n)
else

let totalWeight = totalWeight + assignWeight in

let listOfWeights = listOfWeights@assignWeight in

(grabWeights (n-1)) in


(*function to grab student names and grades*)
fun grabStudents with m =

let numStudents = (numStudents +1) in
let _ = print "Please enter a student" in
let student = readString in
let listOfGrades = _ in
(grabGrades student m listOfGrades)




in


(*function to grab grades of each student*)
fun grabGrades with studentName, k, gradeList =

if k = 0 then
let studentList = studentList@studentName in
let gradeList = gradeList@studentName in

let _ = print "Another student? (y/n)" in
let answer = readString in

if answer="y" then
(grabStudents numAssignments)
else


let totalNumber = (numAssignments+1)*numStudents in

(calcAvg gradeList totalNumber numStudents)

else

let _ = print "Please enter a grade" in
let gradeEntered = readInt in

if gradeEntered > 100 then
let _ = print "You entered too high of a grade! Please enter grade between 0 and 100" in
(grabGrades studentName (k) gradeList)
else

if gradeEntered < 0 then
let _ = print "You entered too low of a grade! Please enter grade between 0 and 100" in
(grabGrades studentName (k) gradeList)
else

let studentList = studentList@(999) in
let gradeList = gradeList@gradeEntered in

(grabGrades studentName (k-1) gradeList) in

(*function to calc averages for each student*)
fun calcAvg with gradeList, totalNum, stuNum = 

if totalNum = 0 then
(sortGrades listOfAvg 100)
else

if #gradeList=#studentList then

let listOfAvg = listOfAvg@#gradeList in
let studentList = !studentList in
let studentList = !studentList in


let gradeList = !gradeList in


let tempListOfWeights = listOfWeights in


let weightedGrade = ((#gradeList * #tempListOfWeights)/100) in
let tempListOfWeights = !tempListOfWeights in
let listOfAvg = listOfAvg@weightedGrade in

let gradeList = !gradeList in


(calcAvg gradeList (totalNum-2) stuNum) 
else

let studentList = !studentList in



let weightedGrade = ((#gradeList * #tempListOfWeights)/100) in
let gradeToAdd = #listOfAvg+weightedGrade in
let listOfAvg = !listOfAvg in
let tempListOfWeights = !tempListOfWeights in
let listOfAvg = listOfAvg@gradeToAdd in

let gradeList = !gradeList in



(calcAvg gradeList (totalNum-1) stuNum) in

fun sortGrades with listOfAvg, countNum =

if countNum < 0 then
let _ = print "The final sorted list is: " in
let _ = print sortedList in
1
else

let templistOfAvg = listOfAvg in
(studentSearch numStudents countNum) in


fun studentSearch with totalStudents, count =

if totalStudents = 0 then


(sortGrades listOfAvg count-1)
else




if #templistOfAvg = count then


let tempAvgToAdd = #templistOfAvg in
let templistOfAvg = !templistOfAvg in

let sortedList = sortedList@(#templistOfAvg@tempAvgToAdd) in


let templistOfAvg = !templistOfAvg in


(studentSearch totalStudents-1 count)
else
let templistOfAvg = !templistOfAvg in
let templistOfAvg = !templistOfAvg in


(studentSearch totalStudents-1 count)

in


(grabWeights numAssignments)

