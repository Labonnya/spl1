#include<iostream>
#include<fstream>
#include<vector>
#include<stdlib.h>
#include<string.h>
#include<cstdio>
#include<iomanip>
#include<queue>
#include<sstream>
#include "key_expand.h"
#include "encoding.h"
#include "decoding.h"
#include <typeinfo>
#include<Windows.h>
using namespace std;

//function prototypes
void registration();
bool login();
void forgot();
void email();
size_t length(const char *s);
void createNewFile();
void writeInFile();
void readFromFile();
void updateFile();
void renameFile();
void copyFile();
void deleteFile();
void searchInFile();
void searchFileInDirectory();
void getFileSizeDirectory();
void clearFile();
void mergeFile();
void menuFirst();
void menuFlyFiler();
void compression();
void decompression();
void encryptionAES();

string str;
int q = 0;
int count3 = 0;
int count1 = 0;
string codes;
int num = 254;
char chh = char(num);
vector<pair<char,string> > charactermap;
vector<int> Huffcode;

//================================================================================================
class BitwiseWrite
{
	private:
	  char buf;
	  int Tbits;
	  ostream& out;
	public:


  BitwiseWrite(ostream & os) : out(os),buf(0),Tbits(0)
  {
 							//clear buffer and bit counter
  }


  void flush();   	 // write buffer into output file and clear it
  void writeBit(int i);
};



void BitwiseWrite::flush() {
  out.put(buf);
  out.flush();
  buf = Tbits = 0;
}

void BitwiseWrite::writeBit(int i) // Write the least significant bit of the argument to the bit buffer
{
	if( i == -1)     //for last bit if buffer is not full still flush buffer
	{
		flush();
	}
  if(Tbits == 8) //if buffer full(8 bit) clear buffer
  {
    flush();
  }

  //if the int is 1, we must put the bit 1 into the buffer
  unsigned char mask = 1;
  if(i == 1)
  {
    mask = mask << (7 - Tbits);
    buf = buf | mask;
  }
  //if the int is 0, we must put the bit 0 into the buffer
  if(i == 0)
  {
    mask = mask << (7 - Tbits);
    mask = ~mask;
    buf = buf & mask;
  }
  Tbits++;                   //increment Tbits++
}
//===================================================================================================




struct MinHeapNode {

    // One of the input characters
    int data;

    // Frequency of the character
    int freq;

    // Left and right child
    MinHeapNode *left, *right;

    MinHeapNode(int data, int freq)
    {

        left = right = NULL;
        this->data = data;
        this->freq = freq;
    }
};

// For comparison of
// two heap nodes (needed in min heap)
struct compare {

    bool operator()(MinHeapNode* l, MinHeapNode* r)

    {
        return (l->freq > r->freq);
    }
};

// Prints huffman codes from
// the root of Huffman Tree.
int p;
string str1= "";
int n=0;


void printCodes(struct MinHeapNode* root, string str)
{

    if (!root)
        return;

    if (root->data != chh)
    {
		char ch;
		ch = char(root->data);
        cout <<"\t    "<< ch << "\t\t\t  " << str << "\n";
	if(root->data == ' ')
	{
		codes = codes + "_" + " " + str + "Î";
	}
	else
		codes = codes + ch + " " + str + "Î";
	for(int i = 0;i < count3;i++)
	{

		if(charactermap[i].first == ch)
		{
			charactermap[i].second = str;
		}
  	 }
     }
    printCodes(root->left, str + "0");
    printCodes(root->right, str + "1");
}
void printTree(ostream & out,struct MinHeapNode* root,int indent)
{
	if (root != NULL)
	{
		printTree(out, root->right, indent + 4);
		out << setw(indent) << " " << (char)root->data << endl;
		printTree(out, root->left, indent + 4);
	}
}
// The main function that builds a Huffman Tree and
// print codes by traversing the built Huffman Tree
priority_queue<MinHeapNode*, vector<MinHeapNode*>, compare> minHeap;
void HuffmanCodes(int data[], int freq[], int size)
{
    struct MinHeapNode *left, *right, *top;

    // Create a min heap & inserts all characters of data[]


    for (int i = 0; i < size; ++i)                                //Build Heap
        minHeap.push(new MinHeapNode(data[i], freq[i]));

    // Iterate while size of heap doesn't become 1
    while (minHeap.size() != 1) {

        // Extract the two minimum
        // freq items from min heap
        left = minHeap.top();
        minHeap.pop();

        right = minHeap.top();
        minHeap.pop();

        // Create a new internal node with
        // frequency equal to the sum of the
        // two nodes frequencies. Make the
        // two extracted node as left and right children
        // of this new node. Add this node
        // to the min heap 'â– ' is a special value
        // for internal nodes, not used
        top = new MinHeapNode(chh, left->freq + right->freq);

        top->left = left;
        top->right = right;
        minHeap.push(top);
    }

    // Print Huffman codes using
    // the Huffman tree built above
    printCodes(minHeap.top(),"");
}
int total_bits_written()
{
	string temp;
	int total_bits = 0;
	for(int i=0; i < charactermap.size();i++)
	{
		temp = charactermap[i].second;
		for(int j=0; j < temp.length();j++)
		{
			total_bits++;
		}
	}
	return total_bits;
}


int ct = 0;
void cal_compressionRatio()
{
	long double csize = 0;
	long double size = 0;
	long double cratio;
	long long lsize = 0;
	long long lcsize = 0;
	char ch;
	int x;
	x = count3;
	x = x - 1;
	lsize = x;
	lcsize = Huffcode.size()/8;
	size = x * 8;
	size = size /8;
	cout<<"\nActual Size of File  = "<<lsize<<" Bytes";
	csize = Huffcode.size()/8;
	cout<<"\nSize of Compressed File = "<<lcsize<<" Bytes";
	cratio = (csize/size)*100;
	cout<<"\nCompression Ratio achieved : "<<cratio<<"% !";
}
void calculate_frequency(int frequency[],char filename[])
{
    char ch,ch1;
    int y;
    int f;
   int x;
    ifstream fin;
    fin.open(filename);
	if(!fin.is_open())
	{
		cout <<"Error !!! Cannot open Source file.\n";
		cin>>y;
		//exit(1);
	}
    while(fin.get(ch))
    {

	if( ch >=32 && ch < 255)
	{
		charactermap.push_back(make_pair(ch,""));
		count3++;
	}
        y = int(ch);
        frequency[y]=frequency[y]+1;
    }
     ch1 = char(129);
    charactermap.push_back(make_pair(ch1,""));
    count3++;
    fin.close();
}

void write_codes()     //creates codes file
{
	ofstream out;
	out.open("codes.txt");
	for(int i=0; i < codes.size();i++)
	{
		if(codes[i] != 'Î')
		{
			out<<codes[i];

		}
		else
			out<<endl;
	}
	out.close();
}

void SeperateCharacter(int frequency[],int actual_frequency[],int ASCII_values[])
{
	int k =0;
   	 int y=0;
   	 for(int j=32;j < 254;j++)   //For seperating occurring and non-occurring characters & to create actual
   	 {			             // frequency array & ASCII_values array
		if(frequency[j] != 0)
        	{
          	  actual_frequency[k] = frequency[j];
           	 ASCII_values[y] = j;
           	 y++;
           	 k++;
           	 count1++;
		}
    	}

	actual_frequency[y] = 1;     // For adding pseudo-EOF in actual_frequency & ASCII_values arrays
	ASCII_values[k] = 129;
	count1++;
}

void Convert_StringBits_to_IntegerBits()
{
	string temp;
	for (int i = 0; i < charactermap.size();i++)
   	 {
		temp = charactermap[i].second;
		for(int j=0;j < temp.size();j++)          //For storing strings bits into integer vector,for 								   writing bit by bit in file
		{
			if(temp[j] == '1')
			{
				Huffcode.push_back(1);
			}
			else
			{
				Huffcode.push_back(0);
			}
		}
     		//cout  <<"\t"<< charactermap[i].first <<"\t"<< charactermap[i].second<<"\n";
  	  }
	Huffcode.push_back(-1);
}
void printFrequencyTable(int actual_frequency[],int ASCII_values[])
{
	char ch;
	string str = "";
	str = str+"Space";
	cout<<"\n\tFrequency Table of Characters present in file\n\n";
	cout<<"\t===================================================\n";
	cout<<"\tCharacters\t\tFrequency\n";
	cout<<"\t===================================================\n";
	for(int i=0; i < count1; i++)
	{
		ch = char(ASCII_values[i]);
		if(ch == ' ')
			cout<<"\t    "<<str<<"\t\t  "<<actual_frequency[i]<<"\n";
		else
			cout<<"\t    "<<ch<<"\t\t\t  "<<actual_frequency[i]<<"\n";
	}
}
string output;
int count = 0;
unsigned long int total_traversal = 0;
//=======================================================
class BitwiseRead {
private:
  char buf;
  //int first;
  int nbits;        //how many bits have been read from the buffer
  istream& in;
public:

  /* Initialize a BitwiseRead that will use the given istream for input
 * */
  BitwiseRead(istream & is);

  /* Fill the buffer from the input */
  void fill();

  /* Read the next bit from the bit buffer. Fill the buffer form the input
 * stream first if needed.
 * Return 1 if the bit read is 1, 0 if bit read is 0
 * */
  int readBit();
};

BitwiseRead::BitwiseRead(istream & is) : in(is) {
    buf = 0;
    nbits = 8;
}



void BitwiseRead::fill()
{
	buf = in.get();
	nbits = 0;
}

int BitwiseRead::readBit()
{
  if(nbits == 8) {
    fill();
 }

  //uses a mask to extract the nbits'th bit
  unsigned char mask = 1;
  mask = mask << (7-nbits);
  mask = mask & buf;
  nbits++;
  if(mask == 0) {
    return 0;
  }
  else {
    return 1;
  }
}
//====================================================
class Huffman
{
	private:
	/*** Node structure ***/
	class node
	{
		public:
		char data;
		node * left,* right;

		node(char item)   // node constructor
		{
			data = item;
			left = right = 0;
		}
	};
	typedef node * nodePointer;
	public:
		Huffman();
		void buildDecodingTree(ifstream & codeIn);
		void insert(char ch, string code);
		char decode(BitwiseRead &in);
		void printTree(ostream & out, nodePointer root, int indent);
		void displayDecodingTree(ostream & out);
		private:
		nodePointer root;
	};
inline Huffman::Huffman()
{
	root = new node('*');
}
inline void Huffman::displayDecodingTree(ostream & out)
{
	printTree(out, root, 0);
}
//======================================================================================
void Huffman::buildDecodingTree(ifstream & codeIn)
{
	char ch;
	string code;
	for (;;)
	{
		if ( codeIn.eof() ) return;
		codeIn >> ch >> code;
			cout<<"\n"<<ch<<" = "<<code;
			if(ch == '_')
			{
				ch = ' ';
			}
			insert(ch, code);
	}
}


void Huffman::insert(char ch, string code)          //traverses each character from file from its huffman code
{
	Huffman::nodePointer p = root;
	for(int i = 0; i < code.length(); i++)
	{
		switch (code[i])
		{
		case '0' :
				if (p->left == 0) // create node along path
				p->left = new Huffman::node('*');
				p = p->left;
				break;
		case '1' :
				if (p->right == 0) // create node along path
				p->right = new Huffman::node('*');
				p = p->right;
				break;
		default:
			cerr << "*** Illegal character in code ***\n";
			exit(1);
		}
	}
	p->data = ch;
}


char Huffman::decode(BitwiseRead &in)             //To decode from huffman tree
{
	Huffman::nodePointer p;
	p = root;
	int bit;
	while(true)
	{
		bit = in.readBit();
		//count++;
		if(bit == 1)
		{
			p = p -> right;
		}
		if(bit == 0)
		{
			p = p -> left;
		}
		if(p->right == NULL || p->left == NULL)
		{
			break;
		}
	}
	total_traversal++;
	return (p->data);
}

void Huffman::printTree(ostream & out, Huffman::nodePointer root,int indent)  //To print huffman tree
{
	if (root != 0)
	{
		printTree(out, root->right, indent + 4);
		out << setw(indent) << " " << root->data << endl;
		printTree(out, root->left, indent + 4);
	}
}
bool isChar(char c)
{
    return ((c >= 'a' && c <= 'z')
            || (c >= 'A' && c <= 'Z'));
}

bool isDigit(const char c)
{
    return (c >= '0' && c <= '9');
}

int main()
{
    menuFirst();
    menuFlyFiler();
}

void menuFlyFiler()
{
    int choice;
    cout<<"\n\n\n\n";
    cout<<"\t\t\t________________________________________________\n\n\n"<<endl;
    cout<<"\t\t\t             WELCOME TO FLYFILER\n\n";
    cout<<"\n";
    cout<<"\t\t\t___________________Menu___________________\n\n";
    cout<<"\t1. Create new file"<<endl;
    cout<<"\t2. Write in file"<<endl;
    cout<<"\t3. Read from a file"<<endl;
    cout<<"\t4. Update a file"<<endl;
    cout<<"\t5. Rename a file"<<endl;
    cout<<"\t6. Copy contents of a file to another file"<<endl;
    cout<<"\t7. Delete a file"<<endl;
    cout<<"\t8. Merge contents of two file"<<endl;
    cout<<"\t9. List all files in directory"<<endl;
    cout<<"\t10. Get size of all files in directory"<<endl;
    cout<<"\t11. Clear file"<<endl;
    cout<<"\t12. Search in file"<<endl;
    cout<<"\t13. Compress a file"<<endl;
    cout<<"\t14. Decompress a file"<<endl;
    cout<<"\t15. Encrypt or decrypt a file"<<endl;
    cout<<"\t16. Exit"<<endl;

    cout<<"Enter your choice: ";
    cin>>choice;

    switch(choice)
    {
    case 1:
        {
          createNewFile();
            break;
        }
    case 2:
        {
            writeInFile();
              break;
        }
    case 3:
        {
            readFromFile();
              break;
        }

    case 4:
        {
            updateFile();
            break;
        }

    case 5:
        {
            renameFile();
            break;
        }
    case 6:
        {
            copyFile();
        }
    case 7:
        {
            deleteFile();
        }
   /* case 8:
        {
            renameFile();
        }*/
    case 8:
        {
            mergeFile();
        }
    case 9:
        {
            searchFileInDirectory();
        }
    case 10:
        {
            getFileSizeDirectory();
        }
    case 11:
        {
            clearFile();
        }
    case 12:
        {
            searchInFile();
        }
    case 13:
        {
            compression();
        }
    case 14:
        {
           decompression();
        }
    case 15:
        {
          encryptionAES();
        }
    case 16:
        {
            cout<<"\t\t\t Thank You!! \n\n";
              break;
        }

    default:
        system("CLS");
        cout<<"\t\t\t Please select from the given tab. \n";
        menuFlyFiler();
    }
}
void encryptionAES()
{
    //we will read from file input.txt
int extendedlength=0;
int choice;
string myText;
label:
   cout<<"Welcome to 128 bits AES encryption"<<endl;
   cout<<endl;
   cout<<"Enter you choice "<<endl;
   cout<<"1- Encoding"<<endl;
   cout<<"2- Decoding"<<endl;
   cin>>choice;

  switch(choice)
  {
  	case 1:
  		{
  	//encryption of text data
   ifstream File;
   string filepath = "encryption.aes";
    //clearing encryption.aes before editing
   File.open(filepath.c_str(), std::ifstream::out | std::ifstream::trunc );
   if (!File.is_open() || File.fail())
   {
   File.close();
   printf("\nError : failed to erase file content !");
   }
   File.close();
   //reading plain text from input.txt
  	fstream newfile;
	newfile.open("input.txt",ios::in); //open a file to perform read operation using file object
   if (newfile.is_open()){   //checking whether the file is open
      cout<<"Reading plain text from input.txt .........\n";
      Sleep(1000);
      string tp;
       cout<<"Reading KEY from key.txt ......\n";
      Sleep(1000);
       cout<<"Now encrypting ....\n";
          Sleep(1000);
        cout<<"writing encrypted data in encryption.aes ..\n";
          Sleep(1000);
          cout<<endl;
      while(getline(newfile, tp)){
	  //read data from file object and put it into string.
	  int messlength=tp.length();
	  int extendedlength;
	   if((messlength%16)!=0)
	   {
	  	extendedlength=messlength+(16-(messlength%16));
	   }
	   else
	   {
	   	extendedlength=messlength;
	    }
	    unsigned char* encryptedtext=new unsigned char[extendedlength];
	    for(int i=0;i<extendedlength;i++)
         {
  	       if(i<messlength)
  	       encryptedtext[i]=tp[i];
  	       else
  	       encryptedtext[i]=0;
         }
         //getting key from key.txt
        string k;
	    ifstream infile;
	    infile.open("key.txt");
	    if (infile.is_open())
	   {
		getline(infile, k); // The first line of file should be the key
		infile.close();
	   }

	   else cout << "Unable to open file";

	    istringstream tempkey(k);
	    unsigned char key[16];
	    unsigned int x;
	    for(int i=0;i<16;i++)
	    {
	    tempkey>>hex>>x;
		key[i] = x;
	    }
	    //extending key
	     unsigned char extendedkeys[176];
         Key_extenxion(key,extendedkeys);

        //encrypting our plain text
         for(int i=0;i<extendedlength;i+=16)
         {
          unsigned char* temp=new unsigned char[16];
		  for(int j=0;j<16;j++)
		  {
		  	temp[j]=encryptedtext[i+j];
		  }
		  encryption(temp , extendedkeys);
		  for(int j=0;j<16;j++)
		  {
		  	encryptedtext[i+j]=temp[j];
		  }
		 }
	//storing our encrypted data in encryption.aes
	ofstream fout;  // Create Object of Ofstream
    ifstream fin;
    fin.open("encryption.aes");
    fout.open ("encryption.aes",ios::app); // Append mode
    if(fin.is_open())
        fout<<encryptedtext<<"\n"; // Writing data to file
    fin.close();
    fout.close();
      }
      cout<<"128-bit AES encryption is done sucessfully\n";
      cout<<"Data has been appended to file encryption.aes";
      newfile.close(); //close the file object.
   }
  break;
}

	case 2:
		{
	  cout<<"Reading encrypted data from encryption.txt .........\n";
      Sleep(1000);
      string tp;
      cout<<"Reading KEY from key.txt ......\n";
      Sleep(1000);
      cout<<"Now Decrypting ....\n";
      Sleep(1000);
       cout<<"writing decrypted data in outputtext.txt ..\n";
          Sleep(1000);
          cout<<endl;
	cout<<"Following is our decrypted text:- \n";
	//clearing outputtext file
	ifstream File;
   string filepath = "outputtext.txt";
   File.open(filepath.c_str(), std::ifstream::out | std::ifstream::trunc );
   if (!File.is_open() || File.fail())
   {
   File.close();
   printf("\nError : failed to erase file content !");
   }
   File.close();

	ifstream MyReadFile;
	MyReadFile.open("encryption.aes", ios::in | ios::binary);
    if(MyReadFile.is_open())
    {
    while( getline (MyReadFile, myText) )
   {
   	cout.flush();
     char * x;
      x=&myText[0];
      int messlength=strlen(x);
	 char * msg = new char[myText.size()+1];

	strcpy(msg, myText.c_str());

	int n = strlen((const char*)msg);
	unsigned char * decryptedtext = new unsigned char[n];
	//decrypting our encrypted data
	for (int i = 0; i < n; i++) {
		decryptedtext[i] = (unsigned char)msg[i];
	}
	//reading key from key.txt file
		 string k;
	    ifstream infile;
	    infile.open("key.txt");
	    if (infile.is_open())
	   {
		getline(infile, k); // The first line of file should be the key
		infile.close();
	   }

	   else cout << "Unable to open file";
	    istringstream tempkey(k);
	    unsigned char key[16];
	    unsigned int x1;
	    for(int i=0;i<16;i++)
	    {
	    tempkey>>hex>>x1;
		key[i] = x1;
	    }
	    //extending key
        unsigned char extendedkeys[176];
        Key_extenxion(key,extendedkeys);
      //decrypting our data
      for (int i = 0; i < messlength; i += 16)
      {
        unsigned char * temp=new unsigned char[16];
        for(int j=0;j<16;j++)
        temp[j]=decryptedtext[i+j];
		decryption(temp , extendedkeys);
		for(int j=0;j<16;j++)
            decryptedtext[i+j]=temp[j];
      }
      //printing our plain text
			for(int i=0;i<messlength;i++)
			{
			cout<<decryptedtext[i];
			if(decryptedtext[i]==0 && decryptedtext[i-1]==0 )
			break;
	     	}
    //storing plain text in outputtext.txt file
			  cout<<endl;
			  ofstream fout;  // Create Object of Ofstream
              ifstream fin;
              fin.open("outputtext.txt");
              fout.open ("outputtext.txt",ios::app); // Append mode
              if(fin.is_open())
                fout<<decryptedtext<<"\n"; // Writing data to file

             fin.close();
             fout.close(); // Closing the file
         Sleep(500);
		}
}
	else
         {
	      cout<<"Can not open input file\n ";
       }
       cout<<"\n Data has been appended to file outputtext.txt";
MyReadFile.close();
}
  }
}

void createNewFile()
{
    system("CLS");
    string name;
    ofstream file;

    cout<<"Enter your file name: "<<endl;
    cin>>name;

     while(1){
      if(name.find('.'))
      {
          break;
      }
      else{
        cout<<"Not in correct format. Enter again."<<endl;
        cout<<"Enter file name in .txt format: "<<endl;
        cin>>name;
      }
    }

    do
        {
            file.open(name.c_str());
    }while(file.fail());
    system("CLS");
    cout<<"File is successfully created."<<endl;

    file.close();

           menuFlyFiler();

}
void compression()
{
     int frequency[256];
	int ASCII_values[256];
	int actual_frequency[256];
	char ch;
	char filename[50];
	char cfilename[50];
	string decode;
	cout<<"\n\t====================================";
	cout<<"\n\t\t  Text File Compressor\n";
	cout<<"\t====================================";
	cout<<"\n\nEnter Name of File to Compress : ";
	cin>>filename;

   	for(int i=0;i < 255;i++)     //To set initial values of arrays
  	{
     		ASCII_values[i] = 0;
    	 	actual_frequency[i] = 0;
    	 	frequency[i] = 0;
   	}
   	calculate_frequency(frequency,filename);  	//To calculate frequency of each character in file

   	SeperateCharacter(frequency,actual_frequency,ASCII_values);  //Make actual frequency array and ASCII values

	printFrequencyTable(actual_frequency,ASCII_values);  // Print frequency table
	cout<<"\nCharacter Table with Huffman codes \n";
	cout<<"\n\tCharacter\t\tHuffman Code\n";

   	HuffmanCodes(ASCII_values,actual_frequency,count1);  //Function to build Huffman tree

	Convert_StringBits_to_IntegerBits();          //Converts character from charactermap to int vector

	int len = strlen(filename);
	filename[len-1] = 'p';
	filename[len-2] = 'm';
	filename[len-3] = 'c';
	cout<<"\n";
	cout<<"\nHuffman encoding Tree : \n";
	printTree(cout,minHeap.top(),0);      //To display formed Tree  with 'â– ' as parent nodes(including root)
	write_codes();
	ofstream fout;
  	fout.open(filename,ios::binary);

	cal_compressionRatio();                //To calculate compression ratio
	int temp1;
	BitwiseWrite s(fout);
	cout<<"\nProcessing.... Plz wait !!!";
	for(int d = 0;d < Huffcode.size();d++) {
	    temp1 = Huffcode[d];
	    s.writeBit(temp1);                //To write Bit by bit by bit in compressed file
	  }
	cout<<"\nCompression Successful !!!";
	/*int v;
	cout<<"\nEnter any key to exit : ";
	cin>>v;
    	return 0;*/
}
void decompression()
{
    int total_bits_written;
	int current_bits = 0;
	char data;
	char filename[32];
	cout<<"\n\t====================================";
	cout<<"\n\t\t Text File Decompressor\n";
	cout<<"\t====================================";
	cout << "\n\nEnter name of code file: ";
	cin >> filename;
	ifstream codestream(filename);
	if (!codestream.is_open())
	{
		cout <<"Error !!! Cannot open code file.\n";
		exit(1);
	}
	Huffman h;
	h.buildDecodingTree(codestream);
	cout << "Here is the Huffman decoding tree:\n";
	h.displayDecodingTree(cout);
	cout << endl;
	cout << "\nEnter Name of Compressed file : ";
	cin >> filename;
	ifstream in;
	ofstream fout;
	fout.open("Decompressed.txt");
	in.open(filename,ios::binary);
	BitwiseRead os(in);
	if (!in.is_open())
	{
		cout << "Error !!! Cannot open Compressed file.\n";
		exit(1);
	}
	cout<<"\nProcessing..... Plz Wait";
	char ch3;
	ch3=char(129);
	while(true)
	{

		data = h.decode(os);
		if( data == ch3)
		{
			break;
		}
		current_bits++;
		fout << data;
	}
	//cout<<"\nCount = "<<count;
	cout<<"\nDecompression Successful !!!\n";
	cout<<"\nTotal Number of Times Tree Traversed : "<<total_traversal;
}
void writeInFile()
{
        system("CLS");
        string filename,str;
        ofstream file;

        cout<<"Enter the name of the file where you want to write: "<<endl;
        cin>>filename;

        file.open(filename.c_str());
        if(!file)
    {
        cout<<"Error in opening source file. Try again.";

           menuFlyFiler();
    }

        cout<<"Enter the text you want to write in your file: "<<endl;
        //cin>>str;
        cin.clear();
        cin.sync();
        getline(cin,str);

        file<<str<<endl;
        system("CLS");
        cout<<"File is written successfully."<<endl;
        file.close();

           menuFlyFiler();

}

void readFromFile()
{
    system("CLS");
    string filename,str;
        ifstream file;

        cout<<"Enter the name of the file you want read: "<<endl;
        cin>>filename;

        cout<<"\n";
        cout<<"Content of the file: "<<endl;

        file.open(filename.c_str());

        if(!file)
    {
        cout<<"Error in opening source file. Try again.";
        main();
    }
        while(getline(file,str)){
            cout<<str<<endl;
        }
        //system("CLS");
           menuFlyFiler();
}

void updateFile()
{
    system("CLS");
    string filename,str;
    int n;
        ofstream file;
        ifstream file1;

        cout<<"Enter the name of the file which you want to update: "<<endl;
        cin>>filename;

        file.open(filename.c_str(),ios::out|ios::app);
        if(!file)
    {
        cout<<"Error in opening source file. Try again.";
        main();
    }

        cout<<"Enter the text you want to write in your file: "<<endl;

        cin.clear();
        cin.sync();
        getline(cin,str);

        file<<str<<endl;

        cout<<"Do you want to check if your file is updated? If yes press 1 else press 0."<<endl;
        cin>>n;
        if(n==1)
        {
            cout<<"Content of the file: "<<endl;

        file1.open(filename.c_str());
        while(getline(file1,str)){
            cout<<str<<endl;
        }

           menuFlyFiler();
        file.close();
        }
        else if(n==0)
        {

           menuFlyFiler();
        }
}

void copyFile()
{
    system("CLS");
    ifstream file1;
    ofstream file2;

    char ch;
    string fname1,fname2;

    cout<<"Enter the name of the file from which you want to copy: "<<endl;
    cin>>fname1;

    file1.open(fname1.c_str());
    if(!file1)
    {
        cout<<"Error in opening source file. Try again.";

           menuFlyFiler();
    }
    cout<<"Enter the name of the file where you want to copy: "<<endl;
    cin>>fname2;

    file2.open(fname2.c_str());
    if(!file2)
    {
        cout<<"Error in opening destination file. Try again.";

           menuFlyFiler();
    }

    while(!file1.eof())
        {
           ch=(char)file1.get();
           if(int(ch)!=-1 )
           {
               file2<<ch;
           }
        }
        system("CLS");
    cout<<"File copied successfully."<<endl;
    file1.close();
    file2.close();

           menuFlyFiler();
}

void deleteFile()
{
    string filename;
        ifstream file;

        cout<<"Enter the name of the file you want to delete: "<<endl;
        cin>>filename;
        if(!file)
    {
        cout<<"Error in opening source file. Try again.";
        main();
    }
        remove(filename.c_str());
        cout<<"File is removed."<<endl;
        menuFlyFiler();
}
void renameFile()
{
    ifstream file1;
      ofstream file2;

    char ch;
    string fname1,fname2;

    cout<<"Enter the name of the file you want to rename: "<<endl;
    cin>>fname1;

    file1.open(fname1.c_str());
    if(!file1)
    {
        cout<<"Error in opening source file. Try again.";
        main();
    }
    cout<<"Enter the name you want to set for your file: "<<endl;
    cin>>fname2;

    file2.open(fname2.c_str());
    if(!file2)
    {
        cout<<"Error in opening destination file. Try again.";
        main();
    }
     while(!file1.eof())
        {
           ch=(char)file1.get();
           if(int(ch)!=-1 )
           {
               file2<<ch;
           }
        }
         file1.close();
        remove(fname1.c_str());
        cout<<"File is renamed."<<endl;
        menuFlyFiler();
        file2.close();
}

void searchInFile()
{

string filename;
        int choice;
        ifstream file;

        cout<<"Enter the name of the file where you want to search: ";
        cin>>filename;

        file.open(filename.c_str());
        if(!file)
    {
        cout<<"Error in opening destination file. Try again.";

           menuFlyFiler();
    }

        cout<<"\t\tSearch option menu: "<<endl;
        cout<<"1. Search a word in file and number of occurrence"<<endl;
        cout<<"2. Search a character in file and number of occurrence"<<endl;
        cout<<"3. Find the longest word in file"<<endl;
        cout<<"4. Find the shortest word in file"<<endl;
        cout<<"5. Find number of line in file"<<endl;
        cout<<"6. Find digit in file"<<endl;

        cout<<"Enter your choice: ";
        cin>>choice;

        switch(choice)
        {
        case 1:
            {
                int count=0;
                char ch[20],c[20];

                cout<<"Enter the word you want to search: ";
                cin>>c;

                while(file)
                {
                    file>>ch;
                    if(strcmp(ch,c)==0)
                    {
                        count++;
                    }
                }
                if(count==0)
                {
                    cout<<"Word is not present in file."<<endl;
                }
                else
                {
                    cout<<"Word "<<c<< " is found and occurred "<< count <<" times in file."<<endl;
                }

                //searchInFile();

            }
        case 2:
            {
                   int count=0;
                   char c,ch;
                   cout<<"Enter the character you want to search: "<<endl;
                   cin>>c;
                   while(!file.eof())
                   {
                       file>>ch;
                    if(ch==c)
                    {
                        count++;
                    }
                   }
                   if(count==0)
                {
                    cout<<"Character "<<c<<" is not present in file."<<endl;
                }
                else
                {
                    cout<<"Character is "<<c<< " found and occurred "<< count <<" times in file."<<endl;
                }
                searchInFile();
                break;

            }
        case 3:
            {
                char ch1[100],ch2[100];
                int maximum=0;
                while(!file.eof())
                {
                    file>>ch1;
                    if(maximum<strlen(ch1))
                    {
                        maximum=strlen(ch1);
                        strcpy(ch2,ch1);
                    }
                }
                cout<<"The longest word is " <<ch2<<endl;
                cout<<"Length of the longest word is " <<maximum<<endl;
                searchInFile();
                break;
                 //searchInFile();
            }
        case 4:
            {
                char ch1[100],ch2[100];
                int minimum=1000;
                while(!file.eof())
                {
                    file>>ch1;
                    if(minimum>strlen(ch1))
                    {
                        minimum=strlen(ch1);
                        strcpy(ch2,ch1);
                    }
                }
               // system("CLS");
                cout<<"The shortest word is " <<ch2<<"."<<endl;
                cout<<"Length of the shortest word is " <<minimum<<"."<<endl;
                searchInFile();
                break;
                // searchInFile();
            }
        case 5:
            {
                int count=0;
                   char ch;
                   while(!file.eof())
                   {
                       file.get(ch);
                       if(ch=='.')
                        {
                           count++;
                        }
                   }

                       cout<<"Number of lines in file is "<<count<<"."<<endl;
searchInFile();
                break;
                        //searchInFile();
            }

        case 6:
            {
                int count=0;
                char ch;
                   while(!file.eof())
                   {
                       file.get(ch);
                       if(isdigit(ch))
                        {
                           count++;
                        }
                   }
                        if(count==0)
                {
                    system("CLS");
                    cout<<"Digit is not present in file."<<endl;
                }
                else
                {
                   // system("CLS");
                    cout<<"Digits are found and occurred "<< count <<" times in file."<<endl;
                }
menuFlyFiler();
               break;

                //searchInFile();
           }
           }

}

void searchFileInDirectory()
{
    system("CLS");
   WIN32_FIND_DATAA FindFileData;
    HANDLE hFindFile;
    LPCWSTR file = L"*.cpp";

    hFindFile = FindFirstFile((LPCSTR)file,&FindFileData);

    if(INVALID_HANDLE_VALUE == hFindFile)
    {
        cout<<"Error in finding file."<<endl;
        cout<<"Error "<< GetLastError() << endl;
    }
    else
    {
        cout<<"File found."<<endl;
        wcout<<"File name - " <<FindFileData.cFileName<<endl;
       // wcout<<"File size - " <<FindFileData.nFileSizeLow<<endl;
    }
    while(FindNextFile(hFindFile,&FindFileData))
    {
        wcout<< "File name - " <<FindFileData.cFileName<<endl;
    }

    FindClose(hFindFile);

           menuFlyFiler();

}

void getFileSizeDirectory()
{
    system("CLS");
     WIN32_FIND_DATAA FindFileData;
    HANDLE hFindFile;
    LPCWSTR file = L"*.cpp";

    hFindFile = FindFirstFile((LPCSTR)file,&FindFileData);

    if(INVALID_HANDLE_VALUE == hFindFile)
    {
        cout<<"Error in finding file."<<endl;
        cout<<"Error "<< GetLastError() << endl;
    }
    else
    {
        cout<<"File found."<<endl;
        wcout<<"File name - " <<FindFileData.cFileName<<endl;
        wcout<<"File size - " <<FindFileData.nFileSizeLow<<endl;
    }
   while(FindNextFile(hFindFile,&FindFileData))
    {
        wcout<< "File name - " <<FindFileData.cFileName<<endl;
        wcout<<"File size - " <<FindFileData.nFileSizeLow<<" kb " <<endl;
    }

    FindClose(hFindFile);

           menuFlyFiler();
}

void clearFile()
{
    system("CLS");

    string filename,str;
        ofstream file;

        cout<<"Enter the name of the file which you want to clear: "<<endl;
        cin>>filename;

        file.open(filename.c_str(),std::ofstream::out | std::ofstream::trunc);
        if(!file)
    {
        cout<<"Error in opening source file. Try again.";

           menuFlyFiler();
    }
    file.close();
    system("CLS");
    cout<<"File is cleared."<<endl;

           menuFlyFiler();
}

void mergeFile()
{
system("CLS");
      ifstream file1,file2;
      ofstream file3;

    string str1,str2;
    string fname1,fname2,fname3;

    cout<<"Enter the name of the first file: "<<endl;
    cin>>fname1;

    file1.open(fname1.c_str());
    if(!file1)
    {
        cout<<"Error in opening source file. Try again.";

           menuFlyFiler();
    }

    cout<<"Enter the name of the second file: "<<endl;
    cin>>fname2;

    file2.open(fname2.c_str());
    if(!file2)
    {
        cout<<"Error in opening destination file. Try again.";

           menuFlyFiler();
    }

    cout<<"Enter the new file name where you want to merge: "<<endl;
    cin>>fname3;

    file3.open(fname3.c_str());

    while(getline(file1, str1)){
         file3<<str1;
         file3<<endl;
      }

   while(getline(file2, str2)){
         file3<<str2;
         file3<<endl;
      }

        file1.close();
        file2.close();
        file3.close();
        system("CLS");
        cout<<"File is merged successfully."<<endl;

           menuFlyFiler();
}



void menuFirst()
{
    int choice;
    cout<<"\t\t\t_____________________________________________________\n\n\n";
    cout<<"\t\t\t            WELCOME TO FLYFILER\n\n\n";
    cout<<"\t\t\t___________________Menu___________________\n\n";
    cout<<"\t Press 1 to LOGIN           \n";
    cout<<"\t Press 2 to REGISTRATION          \n";
    cout<<"\t Press 3 if you forgot your PASSWORD \n";
    cout<<"\t Press 4 to EXIT \n";
   // cout<<"\t Press 5 to go to FlyFiler Menu \n";

    cin>>choice;
    cout<<endl;

    switch(choice)
    {
    case 1:
        {
          login();
           break;
        }

    case 2:
        {
           registration();
            break;
        }

    case 3:
        {
             forgot();
               break;
        }

    case 4:
        {
            cout<<"\t\t\t Thank You!! \n\n";
               break;
        }

    /*case 5:
        {
           menuFlyFiler();
        }*/

    default:
        system("CLS");
        cout<<"\t\t\t Please select from the given tab. \n";
        menuFirst();
}
}
bool login()
{
    bool log = false;
    int count;
    string userID,password,id,pass;
    system("CLS");
    cout<<"\t\tPlease enter your username and password: \n";
    cout<<"\t\tUsername: ";
    cin>>userID;
    //cout<<"\n";
    cout<<"\t\tPassword: ";
    cin>>password;

    ifstream input("record.txt");

    while(input>>id>>pass)
    {
        if(id==userID && pass==password)
        {
            count=1;
            system("CLS");
        }
    }

    input.close();

    if(count==1)
    {
        cout<<"\nYour LOGIN is successful!\n";
        log = true;
    }
    else
    {
        cout<<"\t\t Wrong information. Please check your username and password again.\n";
        log = false;
        main();
    }
    if(log==true)
    {
        menuFlyFiler();
    }

}
void registration()
{
    string name,email_id,password,user_name;
    string phone_no;
    cout<<"Fill up the following informations to sign up with us today.";
    cout<<"\n";
    cout<<"\t\tEnter your full name: ";
    cin>>name;
    cout<<"\t\tEnter your username: ";
    cin>>user_name;
    cout<<"\t\tEnter your email id: ";
    cin>>email_id;

    if (!isChar(email_id[0]))
        {
           cout<<"\t\tInvalid email id. Please try again.\n";
           cout<<"\t\tEnter your email id: ";
            cin>>email_id;
        }
    int a=-1, d=-1;
    for (int i=0; i<email_id.length(); i++) {

        if (email_id[i] == '@') {
            a=i;
        }
        else if (email_id[i] == '.') {
            d=i;
        }
    }
    if (a==-1 || d==-1)
    {
        cout<<"\t\tInvalid email id. Please try again.\n";
        cout<<"\t\tEnter your email id: ";
            cin>>email_id;
    }
    if (a>d)
        {
            cout<<"\t\tInvalid email id. Please try again.\n";
            cout<<"\t\tEnter your email id: ";
            cin>>email_id;
        }
    cout<<"\t\tEnter your phone number: ";
    cin>>phone_no;
    int len = length(phone_no.c_str());
    if(phone_no[0]!=0 && phone_no[1]!=1 && len<11)
    {
        cout<<"\t\tInvalid phone number. Try again.";
        cout<<"\t\tEnter phone number: ";
        cin>>phone_no;
    }
    cout<<"\t\t Set up your password: ";
    cin>>password;
    int pass_len= length(password.c_str());
    if(pass_len<8)
    {
        cout<<"\t\tPassword must be of 8 characters. Try again.\n";
        cout<<"\t\tEnter password again: ";
        cin>>password;
    }

    ofstream f1("record.txt", ios::app);
    f1<<user_name<<" "<<password<<endl;

    ofstream f3("info.txt", ios::app);
    f3<<phone_no<<" "<<name<<" "<<email_id;

    system("CLS");
    cout<<"\t\tYour registration is successful. Thank you!!\n";
    menuFirst();

}
size_t length(const char *s)
{
    size_t size = 0;

    while (*s) {
        size += 1;
        s +=1;
    }

    return size;
}

void forgot()
{
   int option;
   system("CLS");
   cout<<"Don't worry if you forgot your password.";
   cout<<"\nPress 1 to search by your username.";
   cout<<"\nPress 2 to go back to main menu.";

   cout<<"\nEnter your choice: ";
   cin>>option;

   switch(option)
   {
   case 1:
       {
    int count=0;
    string suserid,sid,spass;
    cout<<"Enter the username you remember: ";
    cin>>suserid;

    ifstream f2("record.txt");
    while(f2>>sid>>spass)
    {
        if(sid==suserid)
        {
            count=1;
        }
    }
    f2.close();
    if(count==1)
    {
        cout<<"Your account is found!!";
        cout<<"Your password is: "<<spass;
        cout<<endl;
        //system("CLS");
        menuFirst();
    }
    else
    {
        cout<<"Sorry your account was not found.";
        //system("CLS");
        cout<<endl;
        menuFirst();
    }
    break;
   }
   case 2:
    {
        menuFirst();
    }
   default:
    {
        cout<<"Wrong choice. Please try again."<<endl;
        forgot();
    }
}
}



