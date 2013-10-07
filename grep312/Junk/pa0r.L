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
let _ = print "weight entered was: " in
let _ = print assignWeight in

let totalWeight = totalWeight + assignWeight in
let _ = print "total weight is: " in
let _ = print totalWeight in

let listOfWeights = listOfWeights@assignWeight in
let _ = print listOfWeights in

(grabWeights (n-1)) in


(*function to grab student names and grades*)
fun grabStudents with m =

let numStudents = (numStudents +1) in
let _ = print " Please enter a student" in
let student = readString in
let listOfGrades = _ in
(grabGrades student m listOfGrades)




in


(*function to grab grades of each student*)
fun grabGrades with studentName, k, gradeList =

if k = 0 then
let studentList = studentList@studentName in
let gradeList = gradeList@studentName in
let _ = print gradeList in
let _ = print "Another student? (y/n)" in
let answer = readString in

if answer="y" then
let _ = print "he said yes!" in
(grabStudents numAssignments)
else
let _ = print "he said no :(" in
let _ = print "num students is" in
let _ = print numStudents in

let _ = print "num assignments is" in
let _ = print numAssignments in
let totalNumber = (numAssignments+1)*numStudents in

let _ = print "total number is" in
let _ = print totalNumber in

(calcAvg gradeList totalNumber numStudents)

else

let _ = print "Please enter a grade" in
let gradeEntered = readInt in

let studentList = studentList@(99999999999999999999999999) in
let gradeList = gradeList@gradeEntered in

(grabGrades studentName (k-1) gradeList) in

(*function to calc averages for each student*)
fun calcAvg with gradeList, totalNum, stuNum = 

if totalNum = 0 then
(sortGrades listOfAvg 100)
else
let _ = print "total num is: " in
let _ = print totalNum in
let _ = print "student list is: " in
let _ = print studentList in

(*------------------*)
if #gradeList=#studentList then
let _ = print "---------------------should be studnet name loop-----------" in
let listOfAvg = listOfAvg@#gradeList in
let studentList = !studentList in
let studentList = !studentList in


let gradeList = !gradeList in

let _ = print "list of avg is" in
let _ = print listOfAvg in

let _ = print "#gradeList is:" in
let _ = print #gradeList in

let tempListOfWeights = listOfWeights in
let _ = print "#tempListOfWeights is " in
let _ = print #tempListOfWeights in

let weightedGrade = ((#gradeList * #tempListOfWeights)/100) in
let tempListOfWeights = !tempListOfWeights in
let listOfAvg = listOfAvg@weightedGrade in

let gradeList = !gradeList in

let _ = print "list of avg is" in
let _ = print listOfAvg in

(calcAvg gradeList (totalNum-2) stuNum) 
else
let _ = print "---------------------should NOT be studnet name loop-----------" in
let studentList = !studentList in


let _ = print "list of avg is" in
let _ = print listOfAvg in

let _ = print "#gradeList is:" in
let _ = print #gradeList in
let _ = print "#tempListOfWeights is " in
let _ = print #tempListOfWeights in

let weightedGrade = ((#gradeList * #tempListOfWeights)/100) in
let gradeToAdd = #listOfAvg+weightedGrade in
let listOfAvg = !listOfAvg in
let tempListOfWeights = !tempListOfWeights in
let listOfAvg = listOfAvg@gradeToAdd in

let gradeList = !gradeList in

let _ = print "list of avg is" in
let _ = print listOfAvg in

(calcAvg gradeList (totalNum-1) stuNum) in

fun sortGrades with listOfAvg, countNum =

if countNum = 0 then
let _ = print "sorted list is" in
let _ = print sortedList in
1
else
let _ = print "it's in teh else )))))(f0()(DF0susja" in
let templistOfAvg = listOfAvg in
(studentSearch numStudents countNum) in


fun studentSearch with totalStudents, count =
let _ = print "temp list of avg is" in
let _ = print templistOfAvg in
if totalStudents = 0 then

let _ = print "total students is 0...listOfAvg is" in
let _ = print listOfAvg in
(sortGrades listOfAvg count-1)
else
let _ = print "count is " in
let _ = print count in

let _ = print "#temp list of avg is " in
let _ = print #templistOfAvg in

if #templistOfAvg = count then
let sortedList = sortedList@#templistOfAvg in
let _ = print "sortedList after One is" in
let _ = print sortedList in
let _ = print "tempList before any " in
let _ = print templistOfAvg in
let _ = print "head of templist of avg " in
let _ = print !templistOfAvg in
let templistOfAvg = !templistOfAvg in
let _ = print "tempList after One " in
let _ = print templistOfAvg in
let sortedList = sortedList@#templistOfAvg in
let _ = print "sorted list after Two " in
let _ = print sortedList in

let templistOfAvg = !templistOfAvg in
let _ = print "tempList after Two " in
let _ = print templistOfAvg in

(studentSearch totalStudents-1 count)
else
let templistOfAvg = !templistOfAvg in
let templistOfAvg = !templistOfAvg in

let _ = print "temp list of avg after assigning head ===== is" in
let _ = print templistOfAvg in

(studentSearch totalStudents-1 count)

in


(grabWeights numAssignments)

