# Idea 
Suppose that you have 100 questions with a long answer to prepare for your exam. Some of these quastions will be asked and will have enough time to prepare the answer. Cheating is very hard because proctors always watch on you. 
That was my case and i decided to hack the system and make an electronic cheatsheet!

# Overview
Instead of most common visual communications with cheatsheets, here you can communicate with your device via unvisible typing on a sensor button and feeling of silent motor vibrations. The button is hidden under your clothes as well as vibro-motor.

The mind of the device in arduino (I use arduino nano) with attached SD-card.
The API of device are 1 hidden sensor button and vibro-motor. The protocol is morse code.
Morse code is quite inefficient protocol, however it is very simple and there are a lot of methodics to learn it for 2 days.

The default idea was found here: https://github.com/AlexGyver/GyverCheat/tree/503511fde1de7588373d659735a34bf7a38df7cb

# How to prepare divice
1. Prepare your .txt file with your exam quastions and answers. Every question-answer pair should be separated by "%". The quastion should be compressed to a 5-letter string, consisting of the first letters of first 5 words in a quastion sentence. After this copy the whole text of answer. The example is provided in "f.txt" (encoding windows-1251)
2. Save the file as "f.txt" near B6.py script
3. Run the script. It will create a folder 'readyFiles' inside sdFile folder. It is your encoded data
4. Put 'readyFiles' folder on SD-card and insert it into SD-holder connected to arduino
5. upload 'morseVoenka.ino' code one your arduino device

# How to use
1. 2 seconds hold the button - turn on/off the device
2. Then input the encoded question (up to 5 letters). Is success - they will be returned before firstly and then the answer. If failed - long signal

# Restrictions and details
* The input file is separated by questions firstly (every folder in 'readyFiles' is an encoded question)
* Every question then is divede on .txt files of length 60 characters at most
* Every .txt file runs for abound 2.5 minutes. So, the overall speed is 15 seconds for a word
* Don't use more than 10 files for a quastion
* Does not support of "Э" letter
* Supports only Russian language, but English is much easier
