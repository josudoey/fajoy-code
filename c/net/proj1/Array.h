#ifndef Array_h
#define Array_h
#ifndef STACK_SIZE
#define STACK_SIZE 4096
#endif
static int Stack_Address[STACK_SIZE];
typedef struct Array{
   struct Array *this;
   void (*free)(struct Array*);
   int **address;
   int length;
   int array_size;
}Array;

void ArrayFree(Array *this){
   free(this->address);
   free(this);
}
Array *newArray(int size){
   Array *this=(Array*)malloc(sizeof(Array));
   this->address=(int**)malloc(sizeof(int*)*size);
   this->length=0;
   this->array_size=size;
   this->free=ArrayFree;
   return this;
}
void _ArrayPush(Array *this,int *ptr){
   *(this->address+this->length)=ptr;
   this->length++;
}
void _ArraySet(Array *this,int index,int *ptr){
   *(this->address+index)=ptr;
}

int *ArrayPop(Array *this){
   if(this->length!=0)
      this->length--;
   return (int*)(this->address+this->length);
}
int *_ArrayGet(Array *this,int index){
   return (int*)(this->address+index);
}
int _ToArray(Array *this,int **ptrArray){
   int size=this->length;
   *ptrArray=(int*)malloc(sizeof(int*)*(size+1));
   int **ptr=(int**)*ptrArray;
   int i=0;
   while(i<size){
     *(ptr)=*(this->address+i++);
     ptr++;
   }
   ptr=NULL;
   return  size;
}
static Array STACK={&STACK,ArrayFree,(int**)Stack_Address,0,STACK_SIZE};
static Array *stack=&STACK;
#define push(s,p) ArrayPush(s,p)
#define pop(s,t) (t)*(ArrayPop(s))
#define GetItem(a,i,t) (t)*((t*)a[i])
#define ArrayPush(a,p) _ArrayPush(a,(int*)p)
#define ArraySet(a,i,p) _ArraySet(a,i,(int*)p)
#define ArrayGet(a,i,t) (t)_ArrayGet(a,i)
#define ToArray(a,na) _ToArray(a,(int**)na)
#endif

