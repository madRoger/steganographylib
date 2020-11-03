# steganographylib
Tiny steganography library based on Qt (5.13 and higher). Licensed under the terms of the MIT license.
 You can hide your secret information in picture(s) and extract back.
```
Example:
-----Single container---------
ImageContainer container(QStringLiteral("picture.png"), QStringLiteral("picture_packed.png"));

//pack "secret" into picture.png and save as picture_packed.png
container.packBytes(QByteArray("secret"));

//unpack "secret" from picture_packed.png
container.unpackBytes(); //return "secret"

Also you can pack/unpack secret file
container.packFile(QStringLiteral("secret.txt"));
container.unpackFile(QStringLiteral("secret_unupacked.txt"));

-----Two containers---------
You can use more than one container for work with one secret bytearray/file
ImageContainer container1(QStringLiteral("picture_1.png"), QStringLiteral("picture_1_packed.png"));
ImageContainer container2(QStringLiteral("picture_2.png"), QStringLiteral("picture_2_packed.png"));
Synchronizer sync;//create containers synchronizer
//set secret bytearray or file
sync.setSource(QByteArray("0123456789"));//or sync.setSource(QStringLiteral("secret.txt"));
//append containers to synchronizer
sync.appendContainer(&container1);
sync.appendContainer(&container2);
//pack your secret into containers
sync.pack();
//unpack your secret from containers
sync.unpackBytes();//or sync.unpackFile(QStringLiteral("secret_unpacked.txt"));
```
