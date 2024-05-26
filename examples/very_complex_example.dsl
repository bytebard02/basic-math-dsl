var int limit = 20
var int sum = 0
var int i = 2
var int j
var int isPrime

while (i < limit):
  isPrime = 1
  j = 2
  var int maxJ = i / 2
  while (j < maxJ+1):
    var int divisionResult = i / j
    if (divisionResult * j ? i):
      isPrime = 0
      maxJ = 0
    j = j + 1

  if (isPrime ? 1):
    sum = sum + i

  i = i + 1

var int tempSum = sum
var int isEven = 1

while (tempSum > 1):
  tempSum = tempSum - 2

if (tempSum ! 0):
  isEven = 0

if (isEven ? 1):
  print "Sum of primes up to "
  print limit
  print " is even."
else:
  print "Sum of primes up to "
  print limit
  print " is odd."

print sum

run
