# -*- coding: utf-8 -*-
"""
Alejandra Villanueva
av1658
CS 4613
Project Two - Digit Recognition 0 - 4 Program
"""
import numpy as np #Matrix Max
from sklearn.metrics import confusion_matrix #USED ONLY FOR CONFUSION MATRIX

"""Reads raw data and changes it into pixels and scales it"""
def readRaw(filename, size):
    currImage = 0
    currByte = 0 
    byte = ""
    data = np.zeros((size, 784)) #Initialize array
    with open(filename, "rb") as f: 
        while byte != b"": #While end of file has not been reached
            byte = f.read(1)
            data[currImage][currByte] = (int.from_bytes(byte,"big")/255) #Scale numbers
            currByte += 1 
            if (currByte % 784 == 0): #Next Image
                currByte = 0
                currImage += 1
    f.close()
    return data

"""Read Labels into array"""
def readLabels(filename):
    data = []
    file = open(filename,"r")
    for line in file:
        ints = [int(num) for num in line.split()]
        data.append(ints)
    
    file.close()    
    return data


"""Activation function"""
def sigmoid(x):
    return 1/(1+np.exp(-x))

"""Derivative of activation function"""    
def d_sigmoid(s):
    return s * (1 - s)

d_sigmoidv = np.vectorize(d_sigmoid) ##Allows arrays to be put into a function

"""THE neural Network"""
class NeuralNetwork(object):
    def __init__(self, units):
        self.inputSize = 784 #Size of input
        self.outputSize = 5 #Size of output
        self.hiddenSize = units #Size of Hidden Layer
        self.learningRate = .1
        
        #Initializing Weights randomly
        self.wIJ = np.random.randn(self.hiddenSize, self.inputSize)
        self.wJI = np.random.randn(self.outputSize, self.hiddenSize)
        
        #Initializing Bias randomly
        self.biasHidden = np.random.randn(self.hiddenSize)
        self.biasOutput = np.random.randn(self.outputSize)
        
        self.meanError = 0
    
    #Calculating Squared Mean Error
    def calcError(self, error):
        return (np.sum(np.square(error)) * .5)
    
    #Getter for error
    def getError(self):
        return self.meanError
        
    #Forward for Testing
    def forward(self, imgSample):
        hiddenLayer = np.zeros(self.hiddenSize) #Hidden Layer Input
        output = np.zeros(self.outputSize) #Output
        
        #Weights * Input for Input -> HiddenLayer
        for i in range(self.hiddenSize):
            for j in range(len(imgSample)):
                hiddenLayer[i] += (self.wIJ[i][j] * float(imgSample[j]))
        for i in range(self.hiddenSize):
            hiddenLayer[i] = sigmoid(hiddenLayer[i] + self.biasHidden[i]) #Adding Bias
        
        #Weights * Input for HiddenLayer -> Output
        for i in range(len(output)):
            for j in range(len(hiddenLayer)):
                output[i] += (self.wJI[i][j] * float(hiddenLayer[j]))
                
        for i in range(len(output)):
            output[i] = sigmoid(output[i]+self.biasOutput[i]) #Adding bias
    
        return output     
    
    def train(self, imgSample, targets):
        #Training the NN 
        hiddenLayer = np.zeros(self.hiddenSize) #Hidden Layer
        output = np.zeros(self.outputSize)  #Output Layer
    
        #Forward Propagation Part
        #Input -> Hidden
        for i in range(self.hiddenSize):
            for j in range(len(imgSample)):
                hiddenLayer[i] += (self.wIJ[i][j] * float(imgSample[j]))
        for i in range(self.hiddenSize):
            hiddenLayer[i] = sigmoid(hiddenLayer[i] + self.biasHidden[i])
        
        #Hidden -> Output
        for i in range(len(output)):
            for j in range(len(hiddenLayer)):
                output[i] += (self.wJI[i][j] * float(hiddenLayer[j]))
                
        for i in range(len(output)):
            output[i] = sigmoid(output[i]+self.biasOutput[i])
            
        #Calculate Output Error
        outputErrors = targets - output
        self.meanError = self.calcError(outputErrors)
        
        #Calculate Gradient Output/Hidden
        gradientsJI = d_sigmoidv(output)
        gradientsJI *= outputErrors
        gradientsJI *= self.learningRate
        self.biasOutput += gradientsJI #updating bias

        gradientsJI = gradientsJI.reshape(-1,1)
        
        wJI_deltas = np.multiply(gradientsJI,hiddenLayer)
        self.wJI += wJI_deltas #updateWeights!
       
        #Calculate Hidden Errors Hidden
        wJI_T = np.transpose(self.wJI)
        hiddenErrors = np.dot(wJI_T, outputErrors)
        
        #Calculate Gradient Input/Hidden
        gradientsIJ = d_sigmoidv(hiddenLayer)
        gradientsIJ *= hiddenErrors
        gradientsIJ *= self.learningRate
        self.biasHidden += gradientsIJ #updating bias

        gradientsIJ = gradientsIJ.reshape(-1,1)

        wIJ_deltas = np.multiply(gradientsIJ,imgSample);
        self.wIJ += wIJ_deltas #updateweights!!    
               
       
testNum = 2562 #Size of Testing Imgs Array. Number explained in txt files
trainNum = 28039 #Size of Traing Imgs Array. Number explained in txt files

#Read in Data!
trainImgs = readRaw("train_images.raw", trainNum)
testImgs = readRaw("test_images.raw", testNum)
                    
trainLbls = readLabels("train_labels.txt")
testLbls = readLabels("test_labels.txt")

def main():
    units = 200 #Number of units in hidden layer
    NN = NeuralNetwork(units) #Making the layer!
    
    #Variables for first type of Trainging
#    random = False
#    prevError = 0
#    diffError = 1
#    currImg = 0 
#    iterations = 0
#    
#    while diffError > 0.00001:
#        if currImg < len(trainLbls) and random == False:
#            NN.train(trainImgs[currImg], trainLbls[currImg])
#            currImg += 1
#            
#        else: #If training data is not enough, use same data but input it randomly
#            random = True
#            currImg = np.random.randint(len(trainLbls))
#            NN.train(trainImgs[currImg], trainLbls[currImg])
#        iterations += 1
#        diffError = abs(NN.getError() - prevError)
#        prevError = NN.getError()
    
    #Training NN with set number of imgs and random inputs
    trainingTimes = 5000 ##TrainingTime max is 28038
    for index in range(trainingTimes):
        NN.train(trainImgs[index], trainLbls[index])

    for index in range(trainingTimes):
        currImg = np.random.randint(len(trainLbls))
        NN.train(trainImgs[currImg], trainLbls[currImg])
   
    #Confusion Matrix!!
    y_true = []
    y_pred = []
    for i in range(len(testLbls)):
        num = testLbls[i].index(1)
        y_true.append(num)
        
        output = NN.forward(testImgs[i])
        pred = np.argmax(output)
        y_pred.append(pred)
    
    print(confusion_matrix(y_true, y_pred, labels = [0,1,2,3,4]))
    
main()

#THE END!