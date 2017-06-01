A brief description what Sudoku solver do:
1) Automatic recognition of sudoku table and digits from digital image using machine learning (MXNET) and computer vision technologies (openCV)
2) Automatic solving using backtrack algorithm

Required tools:
1. Python 2.7
	- Download and install [Python 2.7][python]
	
2. MXNET for python
	- Install MXNET for python:
	- Open command prompt and run
	```cmd
	python -m pip install mxnet
	```
	If you don't have pip for python follow instractons [pip]
	
3. MXNET for Visual Studio 2015
	- Download and unzip [mxnet_x64_vc14_cpu.7z][mxnet_vs14]
	- Download [vc14 base package][mxnet_vs14] and follow instractons
	
4. Visual Studio 2015
	- Download and install [Visual Studio 2015][vs]
	
5. OpenCV 3.1.0 for Visual Studio 2015
	- Download and install [OpenCV 3.1.0][opencv]
	
Steps:
1. Run MNIST\Multilayer_Perceptron.py to download MNIST and train network
	- In folder MNIST open command promt and run:
	```cmd
	python Multilayer_Perceptron.py
	```
2. Link Visual Studio 2015 with OpenCV 3.1.0 and MXNET
	- Go to project properties -> C/C++ -> General
	- In Additional Include Directories add path to:
		mxnet_x64_vc14_cpu\include and opencv3.1.0\build\include
	- Go to project properties -> Librarian -> General
	- In Additional Dependencies add
		opencv_world310d.lib and libmxnet.lib
	- In Additional Library Dependencies add path to:
		mxnet_x64_vc14_cpu\lib and opencv3.1.0\build\x64\vc14\lib
	- In Environment Variables -> Path add path to
		mxnet_x64_vc14_cpu\build and opencv3.1.0\build\x64\vc14\bin
		
3. Run test_sudoku_puzzle
	- Go to project properties -> Debugging
	- In Command Arguments add path to the test image
	- Run test_sudoku_puzzle

Next:
Improve digits recognition by adding digital numbers in training set


[python]: <https://www.python.org/downloads/release/python-2712/>
[pip]: <https://github.com/BurntSushi/nfldb/wiki/Python-&-pip-Windows-installation#pip-install>
[mxnet_vs14]: <https://github.com/yajiedesign/mxnet/releases>
[vs]: <https://www.visualstudio.com/downloads/>
[opencv]: <http://opencv.org/releases.html>