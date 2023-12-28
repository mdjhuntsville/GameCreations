
1. About: PingPongClone was created by me, Matthew Jones, using Visual Studio 2022 Community Edition.  I created it using C++.  There are no royalties, profits, etc. from this source. 

2. Folder Struction:
 /PingPongClone
  	/References
  	/External Dependencies
  	/Header Files (included in github)
   		/framework.h
   		/PingPongClone.h
   		/Resource.h
   		/targetverh.h
	/Resource Files (included in github)
   		/PingPongClone.ico
   		/PingPongClone.rc
   		/small.ico
  	/Source Files (included in github).

3. Setup:
Using VS 2022, I began with the 'Desktop App' option.  This is why there are additional resources and populated code.  My edits/additions are 90% within the Source Files.

4. Notes and room for improvement:
	a. When I originally began working with this project, I moved hWnd out of InitInstance into its parent function.  It didn't affect anything for this project, but deeper edits may, or may not need to take that into account.
	b. I used S & W for left paddle, and Up/Down for right paddle.  Adjustments could be fine-tuned for better usability.  Pressing a rogue key while opponent is moving their paddle could sabatoge them. 
	c. Opening the application goes straight into the game.  Perhaps the addition of a 'start' button, or similar could improve functionality.
