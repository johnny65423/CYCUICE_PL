# include <cstdlib>
# include <vector>
# include <iostream>
# include <stdio.h>
# include <string>
# include <map>
# include <sstream>
# include <iomanip>
# include <math.h>



using namespace std;

enum Type { INT, FLOAT, IDENT, OTHER, QUIT, UNKNOWN };

int gLine = 1 ;              // 「下一個要讀進來的字元」所在的line number
int gColumn = 0 ;            // 「下一個要讀進來的字元」所在的column number
bool gReading = false ;
int gTestNum ;
bool gEnd = false ;
int gCount = 0 ;
bool gVerbose = true ;
struct Token {
  string str ;
  
  int line ;
  int column ;
  int intnum ;
  double doublenum ;
  bool iscomd ;
  Type type ;
  
};

typedef Token * TokenPtr ;

struct Symbol {

  string name ;
  Token * args ;
  Token * info ;
  
};

char Cpeek() {
  // int num = getc( stdin ) ;
  char ch = cin.peek();
  // ungetc(ans, stdin);
  return ch;
} // Cpeek()

bool Isletter( char ch ) {
  if ( ch >= 'A' && ch <= 'Z' ) return true ;
  else if ( ch >= 'a' && ch <= 'z' ) return true ;
  else return false ;
  
}

bool Iswhitespace( char ch ) {
  if ( ch == ' ' ) return true ;
  else if ( ch == '\t' ) return true ;
  else if ( ch == '\n' ) return true ;
  else return false ;

} // Iswhitespace()

bool Isdigit( char ch ) {
  if ( ch >= '0' && ch <= '9' ) return true ;
  else return false ;

} // Isdigit()

bool Isseparators( char ch ) {
  if ( ch == '(' || ch == ')' ) return true ;
  else if ( ch == ';' || ch == '\"'  || ch == '\'' ) return true ;
  else if ( Iswhitespace( ch ) ) return true ;
  else return false ;
  
} // Isseparators() 

Type Numtype( string str ) {
  if ( !Isdigit( str[0] ) && str[0] != '+' && str[0] != '-' && str[0] != '.' )
    return UNKNOWN ;
  
  bool hasdigit = false ;
  
  int dotnum = 0 ;
  if ( str[0] == '.' ) dotnum++ ;
  else if ( Isdigit( str[0] ) ) hasdigit = true ;
  
  for ( int i = 1 ; i < str.size() ; i++ ) {
    if ( str[i] == '.' ) dotnum++ ;
    if ( Isdigit( str[i] ) )
      hasdigit = true ;
    if ( !Isdigit( str[i] ) && str[i] != '.' )
      return UNKNOWN ;
  } // for
  

  if ( !hasdigit ) return UNKNOWN ;
  else if ( dotnum == 0 ) return INT ;
  else if ( dotnum == 1 ) return FLOAT ;
  else return UNKNOWN ;

} // Numtype()


  
string Setstr( string str ) {
  
  if ( str == "nil" || str == "#f"  || str == "()" ) return "nil" ;
  else if ( str == "t" || str == "#t" ) return "#t" ;
  else return str ; 
  
} // Setstr()

string Todouble( double num ) {
  stringstream ss ;
  ss << num ; 
  if ( ss.str().find( "." ) != -1 )
    return ss.str() ;
  else {
    ss << "." ;
    return ss.str() ;
  } // else
} // Todouble()

Token * CreatToken( Token token ) {
  Token * retoken = new Token ;
  retoken->str = token.str ;
  retoken->line = token.line ;
  retoken->column = token.column ;
  retoken->intnum = token.intnum ;
  retoken->doublenum = token.doublenum ;
  retoken->type = token.type ;
  retoken->iscomd = false ;

  
  return retoken ;
} // CreatToken()


int Decodeint( string str ) {
  int positive = 1 ;
  if ( str[0] == '+' )
    str.erase( 0, 1 ) ;
  else if ( str[0] == '-' ) {
    str.erase( 0, 1 ) ;
    positive = -1 ;
  } // else if
  
  int num = atoi( str.c_str() ) * positive ; 
  return num ;
} // Decodeint()

double Decodedouble( string str ) {
  int positive = 1 ;
  if ( str[0] == '+' )
    str.erase( 0, 1 ) ;
  else if ( str[0] == '-' ) {
    // str.erase( 0, 1 ) ;
    positive = -1 ;
  } // else if
  
  if ( str[0] == '.' )
    str = "0" + str ;
  
  string ch = "" ;


  return atof( str.c_str() ) ;
} // Decodedouble()

string Setdoublestr( string str ) {
  
  if ( str[0] == '+' )
    str.erase( 0, 1 ) ;
    
  if ( str[0] == '.' )
    str = "0" + str ;
  else if ( str[0] == '-' && str[1] == '.' ) {
    str.erase( 0, 1 ) ;
    str = "-0" + str ;
  } // else if
      
   
      
  int index = str.find( "." ) ;
  int size = str.size() - 5 ;

  if ( index > size ) {

    while ( index != str.size() - 5 ) {
      str = str + "0" ;  
    } // while
      
  } // if
  else if ( index < size ) {

    while ( index != str.size() - 5 ) {
      str.erase( str.end() - 1 ) ;  
    } // while
      
  } // if
  
  char ch = str[str.size() - 1] ;
  
  if ( ch > '4' ) {
    str.erase( str.end() - 1 ) ;
    double num = atof( str.c_str() ) ;
    num = num + 0.001 ;
    stringstream ss ;
    ss << num ;
    str = Setdoublestr( ss.str() ) ;  
  } // if
  else {
    str.erase( str.end() - 1 ) ;
  } // else
  
  return str ;
} // Setdoublestr()

bool Justdot() {
  char ch = Cpeek() ;
  
  if ( Iswhitespace( ch ) )
    return true ;
  else if ( Isseparators( ch ) )
    return true ;
  else 
    return false ;  
} // Justdot()

double Getnum( Token * temp ) {
  if ( temp->type == INT )
    return ( double ) temp->intnum ;
  else if ( temp->type == FLOAT )
    return temp->doublenum ; 
  else return 0.0 ;

} // Getnum()

class Exception {
public:
  string mname ;
  string merrorstr;
  Token * mhead ;
  
  Exception() {  }

  Exception( string str ) {
    merrorstr = str;
  } // Exception()
  
  

}; // Exception

class Callend : public Exception {
public:
  Callend() {
    mname = "Callend" ;
  } // Callend()
};

class EndOfFileError : public Exception {
public:
  EndOfFileError() {
    mname = "EndOfFileError" ;
    stringstream ss ;

    ss << "ERROR (no more input) : END-OF-FILE encountered" ;

    merrorstr = ss.str() ;
  } // EndOfFileError()
};

class Scanner {
  public:
  char Getch() {
    return mch ;
  } // Getch()
  
  void Getchar() {
    
    int eof = scanf( "%c", &mch ) ;
    if ( eof == EOF ) {

      gEnd = true ;
      mch = '\0' ;
    } // if
      
    gColumn++ ;
    if ( mch == '\n' ) {
      
      gColumn = 0 ;
      gLine++ ;
    } // if()
  
  } // Getchar() 
  
  void Readnwschar() {
    while ( ( mch == ' ' || mch == '\n' || mch == '\t' || mch == '\0' ) && !gEnd ) {
      Getchar() ;
    } // while()
    
  } // Readnwschar() 
  
  void Skipcomment() {
    while ( mch == ';' ) {
      Getchar() ;
      while ( mch != '\n' && !gEnd ) {
        Getchar() ;
      } // while
      
      Readnwschar() ;
    } // while
  } // Skipcomment() 
  
  void ReadSexp( vector<Token> & tokenlist ) {
    Token temp ;
    Readnwschar() ;
    temp = Gettoken() ;

    gReading = true ;

    
  } // ReadSexp()  

  void ReadCmd( vector<Token> & tokenlist ) {
    
    
    Readnwschar() ;
    Token temp = Peektoken() ;

    if ( false ) {
      temp = Gettoken() ;
    } // if
    else if ( temp.str == "quit" ) {
      temp = Gettoken() ;
      throw Callend();
    } // else if
    else {
      temp = Gettoken() ;
      cout << "get :" << temp.str << endl ;
      tokenlist.push_back(temp) ;
    } // else
    gReading = true ;

    
  } // ReadCmd()  
  
  void Test() {
    while (!gEnd) {
      Token t = Gettoken() ;
      cout << t.str << endl ;
    } // while
  } // Test()
  
  private:  
  char mch ; 
  Token Peektoken() {
    
    Token temp = Gettoken() ;
    ungetc( mch, stdin ) ;
    gColumn = temp.column ;
    gLine = temp.line ;
    string str = temp.str ;
    for ( int i = str.size() - 1 ; i >= 0 ; i-- ) {
      ungetc( str[i], stdin ) ;
    } // for 
    Getchar();
    return temp ;
  } // Peektoken()

  Token Gettoken() {
    Token retoken ;

    Readnwschar() ; 
    retoken.column = gColumn ;
    retoken.line = gLine ;
    retoken.str = Gettokenstr() ;
    while ( retoken.str == "//" ) {
      while ( mch != '\n' && !gEnd )
        Getchar() ;
      
      if ( gEnd )  
        throw EndOfFileError() ;
      else
        Readnwschar() ; 
        
      retoken.column = gColumn ;
      retoken.line = gLine ;
      retoken.str = Gettokenstr() ;
    } // while
    
    /*
    if ( retoken.type == INT )
      retoken.intnum = Decodeint( retoken.str ) ;
    else if ( retoken.type == FLOAT ) {
      retoken.doublenum = Decodedouble( retoken.str ) ;
      retoken.str = Setdoublestr( retoken.str ) ;
    
    } // else if
    */  

    return retoken ;
  } // Gettoken()
  
  string Gettokenstr() {
    Readnwschar() ;
    
    if ( Isletter( mch ) || mch == '_' ) {
      return Getstring( mch ) ;
    } // if
    else if ( Isdigit( mch ) || mch == '.' ) {
      return Getnum( mch ) ;
    } // else if
    else {
      return Getothers( mch ) ;
    } // else
  } // Gettokenstr() 
  
  string Getstring( char & mch ) {
    string temp = "" ;
    Token tk ;
    tk.line = gLine ;  
    tk.column = gColumn ;

    while ( Isdigit( mch ) || Isletter( mch ) || mch == '_' ) {
      temp += mch ;
      Getchar() ;
    }
    
    return temp ;
  } // Getstring() 

  string Getnum( char & mch ) {
    string temp = "" ;
    bool dot = false ; 

    while ( Isdigit( mch ) || ( mch == '.' && !dot ) ) {
      if ( mch == '.' )
        dot = true ;
      temp += mch ; 
      Getchar() ;
    } // while
    
    return temp ;
  } // Getseparators() 
  
  string Getothers( char & mch ) {
    string temp = "" ; 
    temp += mch ;
    Getchar() ;
    if( temp == "=" || temp == "!" || temp == "*" || temp == ":" ) {
      if ( mch == '=' ) {
        temp += mch ;
        Getchar() ;
      }
    }
    if( temp == "/" ) {
      if ( mch == '=' || mch == '/' ) {
        temp += mch ;
        Getchar() ;
      }
    }
    else if( temp == "&" ) {
      if ( mch == '&' ) {
        temp += mch ;
        Getchar() ;
      }
    }
    else if( temp == "|" ) {
      if ( mch == '|' ) {
        temp += mch ;
        Getchar() ;
      }
    }
    else if( temp == ">" ) {
      if ( mch == '=' || mch == '>' ) {
        temp += mch ;
        Getchar() ;
      }
    }
    else if( temp == "<" ) {
      if ( mch == '=' || mch == '<' ) {
        temp += mch ;
        Getchar() ;
      }
    }
    else if( temp == "-" ) {
      if ( mch == '>' || mch == '-' || mch == '=' ) {
        temp += mch ;
        Getchar() ;
      }
    }
    else if( temp == "+" ) {
      if ( mch == '=' || mch == '+' ) {
        temp += mch ;
        Getchar() ;
      }
    }
      
    return temp ;
  } // Getothers() 
  
    
  
};

Scanner gScanner ;

class Interpreter{
  public:
   
  
  Interpreter() {
    // mch = '\0' ;
    // gPrinter = Printer() ;
    // gScanner = Scanner() ;
    // gTreemaker = Treemaker() ;
  } // Interpreter() 
  
  void Gettokenlist() {
    
    while ( 0 == 0 ) {
		  mtokenlist.clear() ;
      bool err = false ;
      
      printf( "> " ) ;
      if ( gEnd )
        throw EndOfFileError() ;
      
      gScanner.ReadCmd(  mtokenlist ) ;
      for ( int i ; i < mtokenlist.size() ; i++ ) 
        cout << mtokenlist.at(i).str ;

      gReading = false ;
      

      printf( "\n" ) ;
      
      int c = gColumn ;
      char ch = gScanner.Getch() ;
      while ( Iswhitespace( ch ) && ch != '\n' ) {
        gScanner.Getchar() ;
        ch = gScanner.Getch() ;
      } // while
      /*
      ch = gScanner.Getch() ;
      if ( ch == ';' ) {
        while ( ch != '\n' && !gEnd ) {
          gScanner.Getchar() ;
          ch = gScanner.Getch() ;
        } // while
      } // if
      */
      if ( gScanner.Getch() == '\n' )
        gColumn = 0 ; 
      else
        gColumn = gColumn - c + 1 ;
         

    } // while()
    

  } // Gettokenlist()
  
  private:
    vector < Token > mtokenlist ;
    
};

int main() {
  
  Interpreter interpreter = Interpreter() ;

  gScanner = Scanner() ;
  char t ;
  scanf( "%d",  &gTestNum ) ;
  scanf( "%c",  &t ) ;
  
  printf( "Program starts...\n" ) ;
  try {
    // gScanner.Test();
    interpreter.Gettokenlist();
  } // try
  catch ( Exception e ) {
    cout << e.merrorstr ;
    if ( e.mname == "EndOfFileError" )
      printf( "%s", e.merrorstr.c_str() ) ;
  } // catch

  printf( "\nProgram exits..." ) ;

} // main()
