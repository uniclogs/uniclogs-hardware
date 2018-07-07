#Helper Tool by Georges Oates Larsen

from fractions import Fraction
import math

def AsProperFrac(frac):
	basis = int(math.floor(float(frac)))
	subfrac = frac - Fraction(basis)
	if (subfrac == 0):
		return str(basis)
	else:
		return str(basis) + " " + str(subfrac)
	
while True:
	text = raw_input("Please enter a decimal length in Inches: ")
	try:
		value = float(text)
		lowerx16 = int(math.floor(value * 16))
		higherx16 = int(math.ceil(value * 16))
		closestx16 = int(round(value * 16))
		fractionalL = Fraction(lowerx16, 16)
		fractionalH = Fraction(higherx16, 16)
		fractionalC = Fraction(closestx16, 16)
		print("Round Down: " + AsProperFrac(fractionalL) + " in\tRound Up: " + AsProperFrac(fractionalH) + " in\tRound Closest: " + AsProperFrac(fractionalC) + " in")
	except ValueError:
		print("Not a float")