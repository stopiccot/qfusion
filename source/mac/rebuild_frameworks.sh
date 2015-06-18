TOPDIR=`pwd`/../..
FRAMEWORKS_DIR=$TOPDIR/source/mac/Frameworks

# Clean everything
cd $TOPDIR
git clean -xdf

# Angelscript
rm -rf $FRAMEWORKS_DIR/Angelscript.framework
cd $TOPDIR/libsrcs/angelscript/sdk/angelscript/projects/cmake
cmake -G Xcode -DBUILD_FRAMEWORK=1 -DCMAKE_FRAMEWORK_PATH=$FRAMEWORKS_DIR .
xcodebuild -target angelscript -configuration Release ARCHS=x86_64 ONLY_ACTIVE_ARCH=NO
mv ../../lib/Release/Angelscript.framework $FRAMEWORKS_DIR

# Freetype
rm -rf $FRAMEWORKS_DIR/freetype.framework
cd $TOPDIR/libsrcs/libfreetype
cmake -G Xcode -DBUILD_FRAMEWORK=1 -DCMAKE_FRAMEWORK_PATH=$FRAMEWORKS_DIR .
xcodebuild -target freetype -configuration Release ARCHS=x86_64 ONLY_ACTIVE_ARCH=NO
mv ./Release/freetype.framework/ $FRAMEWORKS_DIR

# Png
rm -rf $FRAMEWORKS_DIR/Png.framework
cd $TOPDIR/libsrcs/libpng
cmake -G Xcode -DPNG_FRAMEWORK=1 -DCMAKE_FRAMEWORK_PATH=$FRAMEWORKS_DIR .
xcodebuild -target png16_framework -configuration Release ARCHS=x86_64 ONLY_ACTIVE_ARCH=NO
mv ./Release/Png.framework/ $FRAMEWORKS_DIR

# Ogg
rm -rf $FRAMEWORKS_DIR/Ogg.framework
cd $TOPDIR/libsrcs/libogg
cmake -G Xcode -DBUILD_FRAMEWORK=1 -DCMAKE_FRAMEWORK_PATH=$FRAMEWORKS_DIR .
xcodebuild -target ogg -configuration Release ARCHS=x86_64 ONLY_ACTIVE_ARCH=NO
mv ./Release/Ogg.framework/ $FRAMEWORKS_DIR

# Vorbis
rm -rf $FRAMEWORKS_DIR/Vorbis.framework
cd $TOPDIR/libsrcs/libvorbis
cmake -G Xcode -DBUILD_FRAMEWORK=1 -DCMAKE_FRAMEWORK_PATH=$FRAMEWORKS_DIR .
xcodebuild -target vorbis -configuration Release ARCHS=x86_64 ONLY_ACTIVE_ARCH=NO
mv ./lib/Release/Vorbis.framework/ $FRAMEWORKS_DIR

# Theora
rm -rf $FRAMEWORKS_DIR/Theora.framework
cd $TOPDIR/libsrcs/libtheora
cmake -G Xcode -DBUILD_FRAMEWORK=1 -DCMAKE_FRAMEWORK_PATH=$FRAMEWORKS_DIR .
xcodebuild -target theora -configuration Release ARCHS=x86_64 ONLY_ACTIVE_ARCH=NO
mv ./lib/Release/Theora.framework/ $FRAMEWORKS_DIR

# Curl

# SDL2

# Jpeg

# Test binaries
otool -L $FRAMEWORKS_DIR/Angelscript.framework/Angelscript
otool -L $FRAMEWORKS_DIR/freetype.framework/freetype
otool -L $FRAMEWORKS_DIR/Png.framework/Png
otool -L $FRAMEWORKS_DIR/Ogg.framework/Ogg
otool -L $FRAMEWORKS_DIR/Vorbis.framework/Vorbis
otool -L $FRAMEWORKS_DIR/Theora.framework/Theora