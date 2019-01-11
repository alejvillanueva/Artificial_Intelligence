# Digit Recognition 0-4

In order to run this code, you need to put this python file in a folder with the following files:
-	test_images.raw
-	test_labels.txt
-	train_images.raw
-	train_labels.txt

Open python file in IDE and run it! 

There are two different ways to train the program. The first one uses the mean square error. I am not sure if I did it correctly as it would stop after an average of 250 iterations. The second way I trained the neural network, you tell it how many images you want to feed to it. It does that twice, once feeding it the images in order of the raw data and the second time randomly. This second way worked best and I kept it at about 5000 times. 
