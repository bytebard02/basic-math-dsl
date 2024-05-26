var int num = 5
var int factorial = 1
var int threshold = 100

while (num > 1):
  factorial = factorial * num
  num = num - 1

if (factorial > threshold):
  print "Factorial is greater than the threshold."
else:
  print "Factorial is less than or equal to the threshold."

print factorial

run
