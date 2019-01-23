# wpn GTK+ client

## Repository

git@gitlab.com:commandus/wpn-gtk.git

wpn

[Project description](https://docs.google.com/document/d/19pM4g-hvx2MUVV2Ggljw1MMTg9tMCEH7aHYYKpJMaWk/edit?usp=sharing)

## Dependencies

```
sudo apt install libgtkmm-3.0-dev
```

## Issues

### GTK+ 3.0 compiled with Protobuf 2.6.1

Runnimg ./wpn-gtk cause error:

```
[libprotobuf FATAL google/protobuf/stubs/common.cc:78] This program was compiled against version 2.6.1 of the Protocol Buffer runtime library, which is not compatible with the installed version (3.1.0).  Contact the program author for an update.  If you compiled the program yourself, make sure that your headers are from the same version of Protocol Buffers as your link-time library.  (Version verification failed in "/build/mir-O8_xaj/mir-0.26.3+16.04.20170605/obj-x86_64-linux-gnu/src/protobuf/mir_protobuf.pb.cc".)
terminate called after throwing an instance of 'google::protobuf::FatalException'
  what():  This program was compiled against version 2.6.1 of the Protocol Buffer runtime library, which is not compatible with the installed version (3.1.0).  Contact the program author for an update.  If you compiled the program yourself, make sure that your headers are from the same version of Protocol Buffers as your link-time library.  (Version verification failed in "/build/mir-O8_xaj/mir-0.26.3+16.04.20170605/obj-x86_64-linux-gnu/src/protobuf/mir_protobuf.pb.cc".)
Aborted (core dumped)
```

Reason:

GTK+ 3.0 compiled with Protobuf 2.6.1, libwpn.a compled withj 3.1.0

Solution:

Rebuild libwpn.la with protobuf 2.6.1

```
cd wpn
cd tools
./install-protobuf-2.6.1.sh
cd ..
make wpn
./wpn --version
make libwpn.la
```

./wpn --version must return:
```
libprotobuf: 2.6.1
```
