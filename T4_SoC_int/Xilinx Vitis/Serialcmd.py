
from this import d
from numpy import full
from serial import Serial
import time
import random
import numpy

class zynqTest():
	def __init__(self, port='COM8', baudrate=115200, vectorSize=16 ,tests=1):
		self.serial = Serial(port=port, baudrate=baudrate,timeout=10.0)
		self.expected = []
		self.vectors = []
		self.tests = tests
		self.vectorSize = vectorSize

	def sendVector(self, vector):
		for element in vector:
			self.serial.write( (str(element)+"\n").encode('ascii'))
			time.sleep(0.01)
	
	def receiveResult(self):
		line = self.serial.readline().decode('ascii')
		print("Test")
		print(line)
		while (line == None or line == "\n"):
			time.sleep(0.2)
		#y_euc, elapsed = line.strip().split(":")[1].split(";")
	#	y_euc, elapsed = line.split(',')
		y_euc = line.strip()
		elapsed = 5
		print("Cycles:")
		print(elapsed)
		print("IP result:")
		print(y_euc)
		return int(elapsed), int(y_euc)

	def readGoldenRef(self, gold):
		with open(gold,"r") as goldenRef:
			for tst in range(self.tests):
				count = 0
				auxVec = []
				while (count < 2*self.vectorSize):
					data = goldenRef.readline().strip("\n")
					#print(count)
					#print(data)
					if(data!=''): auxVec.append(int(data))
					count += 1
				self.expected.append((int(goldenRef.readline().strip("\n"))))
				self.vectors.append(auxVec)
				#print(self.vectors)
				print("Expected result for %d integers in each vector:\n" % (self.vectorSize))
				#print(self.expected)
				
				

	def closeSerial(self):
		self.serial.close()
		self.serial = None

	def runTest(self):
		result = 0
		for tst in range(self.tests):
			self.sendVector(self.vectors[tst])
			elapsed, y_euc= self.receiveResult()
			euc_err = 100 * abs((y_euc-self.expected[tst])/self.expected[tst])
			#mean_err = 100 * abs((y_mean-self.expected[tst][1])/self.expected[tst][1])
			#print("TRIAL: %d,\t Expected: [%.3f]\tGot:[%.3f] Err: [%.6f%%]\n" % (tst,self.expected[tst],y_euc, euc_err))
			print("Expected: [%.3f]\tGot:[%.3f] Err: [%.6f%%]\n" % (self.expected[tst],y_euc, euc_err))
			print("NUMBER OF TICKS : %d" % elapsed)
			if (euc_err > 1):
				result += 1
				print("TRIAL: %d FAILED!!" % tst)

		return result
	def createGoldenRef(self,gold):
		with open(gold,"a+") as GoldenRef:
			GoldenRef.truncate(0)	
			for element in range(2*self.vectorSize):
				value=random.uniform(0,100)	
				GoldenRef.write("%d\n"%value)
			GoldenRef.close()

		with open(gold,"r") as GoldenRef:	
			full_array=GoldenRef.read().split('\n')
			full_array=full_array[:-1]
			#print(full_array)
			a=full_array[0:self.vectorSize]
			b=full_array[self.vectorSize:2*self.vectorSize]
			#print(len(a))
			#print(len(b))
			a=numpy.array(a)
			a=a.astype(int)
			b=numpy.array(b)
			b=b.astype(int)
			#print(a)
			dist = numpy.linalg.norm(a-b) 	
			GoldenRef.close()	
		with open(gold,"a+") as GoldenRef:
			GoldenRef.seek(0,2)
			GoldenRef.write("%d\n"%dist)
			GoldenRef.close()		
	
	def clean_data(self):
		self.expected.clear()
		self.vectors.clear()

if __name__ == "__main__":
	tests=3
	zynqDev = zynqTest('COM6', 115200, 640, 1)
	for i in range(tests):
		zynqDev.createGoldenRef("goldenReference2.dat")
		print("Listening...")
		zynqDev.readGoldenRef("goldenReference2.dat")
		#print(zynqDev.vectors)	
		res = zynqDev.runTest()
		if res:
			print(50 * "*")
			print("*" + 22 * " " + "FAIL" + 22 * " " + "*")
			print(50*"*")
		else:
			print(50 * "*")
			print("*" + 22 * " " + "PASS" + 22 * " " + "*")
			print(50*"*")
		#print(res)
		zynqDev.clean_data()

	zynqDev.closeSerial()
	


