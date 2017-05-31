Required tools:
1. Python 2.7
	Download and install [Python 2.7][python]
	
2. MXNET for python
	Install MXNET for python:
	Open command prompt and run
	```cmd
	python -m pip install mxnet
	```
	If you don't have pip for python follow instractons [pip]
	
3. MXNET for Visual Studio 2015
	Download and unzip mxnet_x64_vc14_cpu.7z [mxnet_vs14]
	
4. Visual Studio 2015
	Download and install [Visual Studio 2015][vs]
	
5. OpenCV 3.1.0 for Visual Studio 2015
	Download and install [OpenCV 3.1.0][opencv]
	
Steps:
1. Run Multilayer_Perceptron.py to download MNIST and train network

2. Link Visual Studio 2015 with OpenCV 3.1.0 and MXNET
	- Go to project properties -> C/C++ -> General
	- In Additional Include Directories add path to:
		mxnet_x64_vc14_cpu\include and opencv3.1.0\build\include
	- Go to project properties -> Librarian -> General
	- In Additional Dependencies add
		opencv_world310d.lib and libmxnet.lib
	- In Additional Library Dependencies add path to:
		mxnet_x64_vc14_cpu\lib and opencv3.1.0\build\x64\vc14\lib
	- In Environment Variables -> Path add to
		mxnet_x64_vc14_cpu\build and opencv3.1.0\build\x64\vc14\bin
		
3. Run test_sudoku_puzzle

Note:
Change path to the test images in test main.cpp files

Next:
Improve digits recognition by adding digital numbers in training set


[python]: <https://www.python.org/downloads/release/python-2712/>
[pip]: <https://github.com/BurntSushi/nfldb/wiki/Python-&-pip-Windows-installation#pip-install>
[mxnet_vs14]: <https://github.com/yajiedesign/mxnet/releases>
[vs]: <https://www.visualstudio.com/downloads/>
[opencv]: <http://opencv.org/releases.html>