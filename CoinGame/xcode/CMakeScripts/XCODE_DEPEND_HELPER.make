# DO NOT EDIT
# This makefile makes sure all linkable targets are
# up-to-date with anything they link to
default:
	echo "Do not invoke directly"

# Rules to remove targets that are older than anything to which they
# link.  This forces Xcode to relink the targets from scratch.  It
# does not seem to check these dependencies itself.
PostBuild.CoinGame.Debug:
/Users/justinsiebenhaar/justinSiebenhaar/CoinGame/xcode/Debug/CoinGame:\
	/usr/local/lib/libsfml-graphics.2.5.1.dylib\
	/usr/local/lib/libsfml-window.2.5.1.dylib\
	/usr/local/lib/libsfml-system.2.5.1.dylib
	/bin/rm -f /Users/justinsiebenhaar/justinSiebenhaar/CoinGame/xcode/Debug/CoinGame


PostBuild.CoinGame.Release:
/Users/justinsiebenhaar/justinSiebenhaar/CoinGame/xcode/Release/CoinGame:\
	/usr/local/lib/libsfml-graphics.2.5.1.dylib\
	/usr/local/lib/libsfml-window.2.5.1.dylib\
	/usr/local/lib/libsfml-system.2.5.1.dylib
	/bin/rm -f /Users/justinsiebenhaar/justinSiebenhaar/CoinGame/xcode/Release/CoinGame


PostBuild.CoinGame.MinSizeRel:
/Users/justinsiebenhaar/justinSiebenhaar/CoinGame/xcode/MinSizeRel/CoinGame:\
	/usr/local/lib/libsfml-graphics.2.5.1.dylib\
	/usr/local/lib/libsfml-window.2.5.1.dylib\
	/usr/local/lib/libsfml-system.2.5.1.dylib
	/bin/rm -f /Users/justinsiebenhaar/justinSiebenhaar/CoinGame/xcode/MinSizeRel/CoinGame


PostBuild.CoinGame.RelWithDebInfo:
/Users/justinsiebenhaar/justinSiebenhaar/CoinGame/xcode/RelWithDebInfo/CoinGame:\
	/usr/local/lib/libsfml-graphics.2.5.1.dylib\
	/usr/local/lib/libsfml-window.2.5.1.dylib\
	/usr/local/lib/libsfml-system.2.5.1.dylib
	/bin/rm -f /Users/justinsiebenhaar/justinSiebenhaar/CoinGame/xcode/RelWithDebInfo/CoinGame




# For each target create a dummy ruleso the target does not have to exist
/usr/local/lib/libsfml-graphics.2.5.1.dylib:
/usr/local/lib/libsfml-system.2.5.1.dylib:
/usr/local/lib/libsfml-window.2.5.1.dylib:
