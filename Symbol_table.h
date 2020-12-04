#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <stdio.h>
#include<string.h>
#include<stdlib.h>
#include <iostream>

using namespace std;

const int MAX_NUM_ENTRIES = 100;

typedef enum
{
  type_integer,
  type_string,
  type_boolean,
  type_float,
  type_none
} j_type;

/* printable versions of their names. */
static char *type_names[] =
  { "integer", "string", "boolean", "float", "none" };

static char *entry_type_names[] =
  { "var", "const", "routine", "undefined"};


typedef enum
{
  ste_var,			// a variable
  ste_const,			//A constant
  ste_routine,			//A routine
  ste_undefined,		// ste_entry
} ste_entry_type;


class TableEntry
{
public:
//   char *name;
  int offset;
  string name;
  TableEntry *next;
  ste_entry_type entry_type;
// User-modifiable fields go here:
  union
  {
// /l .for a variable record its type
    struct
    {
      j_type type;
    } var;
// for a constant record its value
    struct
    {
      int value;
    } constant;
/* for a routine, record formal parameters and result type */
    struct
    {
// SteListCelll  *formals;// will be defined later
      j_type result_type;
    } routine;
  };
};


class SymbolTable
{
/* Hash table */
// private:
  TableEntry* myslots[MAX_NUM_ENTRIES];	//Pointer to hash table array of entries
  int fold_case;		// Non-zero => fold upper to lower case

// Statistics on hash table effectiveness
  int number_entries;		// Number of entries in table
  int number_probes;		// Number of probes into table
  int number_hits;		// Number of probes that immediately found entry
  int max_search_dist;		// Maximum entries searched
  int number_myslots;
// SymbolTable   *next;                 // To be used to create a stack of symbol table

// add your defined functions, e.g hash .

// Externally-visible functions
public:
  SymbolTable (){
      clear_symbol_table();
      max_search_dist = 0;
      fold_case = 0;
  };		// fold_case will be set to zero
  SymbolTable (int flod_case_flag){
      clear_symbol_table();
      max_search_dist = 0;
      fold_case = flod_case_flag;
  }
  void clear_symbol_table (){
      number_entries = 0;
      number_myslots  = 0;
      number_hits = 0;
      max_search_dist = 0;

          for(int i = 0 ; i < MAX_NUM_ENTRIES; i++){
              myslots[i] = NULL;
          }

//      std::cout << "table cleared\n" << std::endl;

  }

  TableEntry *Get_symbol (string entry_id){

      int dest = 0;

      if(fold_case){

  //   int length = strlen(entry->name);
   for(int i = 0 ; i < entry_id.length() ; i++){
        entry_id[i] = tolower(entry_id[i]);
      // std::cout << buf[i] << std::endl;
   }
  //  std::cout << entry->name << std::endl;
  }



      long index = myHash(entry_id);
      // cout << "hash of " << entry_id << "  is :" << index << endl;
      if(myslots[index] == NULL){
//      std::cout << "id doesnt exist" << std::endl;
      return NULL;
  }
  TableEntry * node = myslots[index];

  if(!node->name.compare(entry_id)){
//      std::cout << "found my entry" << std::endl;
      number_hits++;
      return node;
  }
  while (node->name.compare(entry_id) != 0)
  {
    if(node->next == NULL){
//      std::cout << "id doesnt exist" << std::endl;
      return NULL;
    }
   dest++;
   node =  node->next;
  }
      if(max_search_dist < dest)max_search_dist = dest;
//      std::cout << "found my entry" << std::endl;
      return node;
  }
  TableEntry *Put_symbol (TableEntry * entry){


//      cout << "inside Symbo" << endl;
      if(fold_case){
//            cout << "fold case is true" << endl;
      //   int length = strlen(entry->name);
       for(int i = 0 ; i < entry->name.length() ; i++){
            entry->name[i] = tolower(entry->name[i]);
          // std::cout << buf[i] << std::endl;
       }
      //  std::cout << entry->name << std::endl;
      }



//     std::cout << "current ident is "  << entry->name << "current fold is :" << fold_case<<std::endl;

      string id = entry->name;
      long index = myHash(id);


      if(myslots[index] == NULL){
          myslots[index] = entry;
          number_entries++;
          number_myslots++;
          return myslots[index];
      }

      else{
          TableEntry * node = myslots[index];
          if(!node->name.compare(entry->name)){
//          std::cout << "id with name '" << node->name <<"' is alredy defined" << std::endl;
          return NULL;
          }
          while(node->next != NULL){
              node = node->next;
            if(!node->name.compare(entry->name)){
//              std::cout << "id alredy defined" << std::endl;
              return NULL;
          }
          }
          number_entries++;
          node->next = entry;
      }
      return entry;
  }
  void print_symbol_stats (){
      std::cout << "num of entries : " << number_entries<< std::endl;
      std::cout << "num of free myslots  : " << MAX_NUM_ENTRIES - number_myslots << std::endl;
      std::cout << "num of hits : " << number_hits << std::endl;
      std::cout << "max search dest (starting from 0): " << max_search_dist << std::endl;
      std::cout << "\n\n";
  }
  void print_all(){
      for(int i = 0 ; i < MAX_NUM_ENTRIES; i++){
          if(myslots[i] == NULL){
          continue;
          }
          TableEntry * node = myslots[i];
          // std::cout << node->name << std::endl;
          while(1){
              cout <<"ID : "<<node->name << "  ,  ";
              cout <<"Entry type :  " << entry_type_names[node->entry_type] << "  ,  ";

              switch(node->entry_type){
                  case 0:
                      std::cout <<"Value: "<<type_names[node->var.type] << std::endl;
                      break;
                  case 1:
                      std::cout << "Value: " << node->constant.value << std::endl;
                      break;
                  case 2:
                      std::cout  <<"Value: "<< type_names[node->routine.result_type]<< std::endl;
                      break;
                  default:
                      std::cout << "record type not defined" << std::endl;
                      break;
              }

              if(node->next == nullptr)
              break;
              node = node->next;

          }
  }
  }
  long myHash( string str){
      unsigned long   h = 0, high; // h=0, g
      string s =  str;
      int i = 0;
      while ( s[i] != NULL)
      {
          h = ( h << 4 ) + s[i++];  //1/4 of bits
          if ( high = h & 0xF0000000 )
              h ^= high >> 24; // h = h ^ (g >>24)
          h &= ~high; // in book this is h = h^g
      }
      return h%MAX_NUM_ENTRIES;
  }

// will add more functions here in the future
};


//long SymbolTable::myHash( string str )
//{
//    unsigned long   h = 0, high; // h=0, g
//    string s =  str;
//    int i = 0;
//    while ( s[i] != NULL)
//    {
//        h = ( h << 4 ) + s[i++];  //1/4 of bits
//        if ( high = h & 0xF0000000 )
//            h ^= high >> 24; // h = h ^ (g >>24)
//        h &= ~high; // in book this is h = h^g
//    }
//    return h%MAX_NUM_ENTRIES;
//}



//TableEntry* SymbolTable::Put_symbol (TableEntry * entry)
//{

//if(fold_case){

////   int length = strlen(entry->name);
// for(int i = 0 ; i < entry->name.length() ; i++){
//      entry->name[i] = tolower(entry->name[i]);
//    // std::cout << buf[i] << std::endl;
// }
////  std::cout << entry->name << std::endl;
//}



//// std::cout << "current ident is "  << entry->name << std::endl;

//string id = entry->name;
//long index = myHash(id);


//if(myslots[index] == NULL){
//    myslots[index] = entry;
//    number_entries++;
//    number_myslots++;
//    return myslots[index];
//}

//else{
//    TableEntry * node = myslots[index];
//    if(!node->name.compare(entry->name)){
//    std::cout << "id with name '" << node->name <<"' is alredy defined" << std::endl;
//    return node;
//    }
//    while(node->next != NULL){
//        node = node->next;
//      if(!node->name.compare(entry->name)){
//        std::cout << "id alredy defined" << std::endl;
//        return node;
//    }
//    }
//    number_entries++;
//    node->next = entry;
//}
//return entry;
//}


//TableEntry* SymbolTable::Get_symbol (string entry_id){

//    int dest = 0;

//    if(fold_case){

////   int length = strlen(entry->name);
// for(int i = 0 ; i < entry_id.length() ; i++){
//      entry_id[i] = tolower(entry_id[i]);
//    // std::cout << buf[i] << std::endl;
// }
////  std::cout << entry->name << std::endl;
//}



//    long index = myHash(entry_id);
//    // cout << "hash of " << entry_id << "  is :" << index << endl;
//    if(myslots[index] == NULL){
//    std::cout << "id doesnt exist" << std::endl;
//    return NULL;
//}
//TableEntry * node = myslots[index];

//if(!node->name.compare(entry_id)){
//    std::cout << "found my entry" << std::endl;
//    number_hits++;
//    return node;
//}
//while (node->name.compare(entry_id) != 0)
//{
//  if(node->next == NULL){
//    std::cout << "id doesnt exist" << std::endl;
//    return NULL;
//  }
// dest++;
// node =  node->next;
//}
//    if(max_search_dist < dest)max_search_dist = dest;
//    std::cout << "found my entry" << std::endl;
//    return node;
//}



//void SymbolTable::print_all(){
//    for(int i = 0 ; i < MAX_NUM_ENTRIES; i++){
//        if(myslots[i] == NULL)
//        continue;
//        TableEntry * node = myslots[i];
//        // std::cout << node->name << std::endl;
//        while(1){
//            cout <<"ID : "<<node->name << "  ,  ";
//            cout <<"Entry type :  " << entry_type_names[node->entry_type] << "  ,  ";

//            switch(node->entry_type){
//                case 0:
//                    std::cout <<"Value: "<<type_names[node->var.type] << std::endl;
//                    break;
//                case 1:
//                    std::cout << "Value: " << node->constant.value << std::endl;
//                    break;
//                case 2:
//                    std::cout  <<"Value: "<< type_names[node->routine.result_type]<< std::endl;
//                    break;
//                default:
//                    std::cout << "record type not defined" << std::endl;
//                    break;
//            }

//            if(node->next == NULL)
//            break;
//            node = node->next;

//        }
//}
//}

//void SymbolTable :: clear_symbol_table(){

//    number_entries = 0;
//    number_myslots  = 0;
//    number_hits = 0;
//    max_search_dist = 0;

//        for(int i = 0 ; i < MAX_NUM_ENTRIES; i++){
//            myslots[i] = NULL;
//        }

//    std::cout << "table cleared\n" << std::endl;

//}



//void SymbolTable:: print_symbol_stats(){
//    std::cout << "num of entries : " << number_entries<< std::endl;
//    std::cout << "num of free myslots  : " << MAX_NUM_ENTRIES - number_myslots << std::endl;
//    std::cout << "num of hits : " << number_hits << std::endl;
//    std::cout << "max search dest (starting from 0): " << max_search_dist << std::endl;
//    std::cout << "\n\n";

//}


//int main ()
//{

//  SymbolTable * table = new SymbolTable();
//  TableEntry *test = new TableEntry ();

//  test->name = "xy";
//  test->entry_type =   ste_var;
//  test->var.type = type_integer;

//  TableEntry *test2 = new TableEntry ();
//  test2->name = "yx";
//  test2->entry_type =   ste_var;
//  test2->var.type = type_float;

//  TableEntry *test3= new TableEntry ();
//  test3->name = "test";
//  test3->entry_type =   ste_routine;
//  test3->routine.result_type = type_string;

//  TableEntry *test4= new TableEntry ();
//  test4->name = "test2";
//  test4->entry_type = ste_const;
//  test4->constant.value = 556 ;

//  table->Put_symbol(test);
//  table->Put_symbol(test2);
//  table->Put_symbol(test3);
//  table->Put_symbol(test4);

//  table->print_symbol_stats();

//  table->Get_symbol("xy");
//  cout << endl;
//  table->Get_symbol("xyz");
//  cout << endl;


//  table->print_symbol_stats();

//  table->clear_symbol_table();

//  table->print_symbol_stats();

//  table->print_all();




//  return 0;
//}

#endif // SYMBOL_TABLE_H
