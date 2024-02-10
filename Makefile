# © 2024 : github.com/NMoroney
# MIT License 
#

#
# * https://developer.uspto.gov/product/patent-official-gazettes-listing
# * https://github.com/frk1/ziplib/tree/master?tab=readme-ov-file
#

EXE=gazette_to_tsv

OPT=-std=c++11 -O3 -w

ZIPLIB=ziplib-master/
ZIPLIB_CPP=$(ZIPLIB)ZipArchive.cpp $(ZIPLIB)ZipArchiveEntry.cpp $(ZIPLIB)ZipFile.cpp $(ZIPLIB)detail/EndOfCentralDirectoryBlock.cpp $(ZIPLIB)detail/ZipGenericExtraField.cpp $(ZIPLIB)detail/ZipCentralDirectoryFileHeader.cpp $(ZIPLIB)detail/ZipLocalFileHeader.cpp

# using -llzma fails to compile, need to use this version of lzma
#
ZL_LZMA=ziplib-master/extlibs/lzma/unix/liblzma.a

N8M=/Users/nathan/tools/n8m

DIR=/Users/nathan/bulk/patent_gazettes/
GAZETTE=e-OG20240206_1519-1.zip


help :
	./$(EXE)


all :
	g++ $(OPT) -I $(N8M) -I $(ZIPLIB) $(ZL_LZMA) $(ZIPLIB_CPP) $(EXE).cc -lz -lbz2 -o $(EXE)
	./$(EXE) $(DIR) $(GAZETTE)


zip :
	zip -r e-OG20240206_1519-1_ntia.tsv.zip e-OG20240206_1519-1_ntia.tsv


