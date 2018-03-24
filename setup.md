# 安裝CP2102的問題

# Driver Download 

    https://www.silabs.com/products/development-tools/software/usb-to-uart-bridge-vcp-drivers

# ISSUE

mac os x 在升級到10.12之後開始傳出各式個樣的cp210x晶片無法連線。

有人提到和 Apple 的系統政策有關。連結如下。

    https://stackoverflow.com/questions/47109036/cp2102-device-is-not-listed-in-dev-on-macos-10-13

方法如下

    Go to "System Preferences" -> "Security & Privacy"

	In the bottom of the window, you will see a message 
	"System software from developer "SiLabs" was blocked from loading."

	Click on "Allow" button

	Restart your Mac
