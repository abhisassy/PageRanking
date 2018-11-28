#include "suffix_tree.h"

double time_elapsed(struct timespec *start, struct timespec *end) {
	double t;
	t = (end->tv_sec - start->tv_sec); // diff in seconds
	t += (end->tv_nsec - start->tv_nsec) * 0.000000001; //diff in nanoseconds
	return t;
}

int cmpfunc (const void * a, const void * b){

   return ( *(DBL_WORD*)a - *(DBL_WORD*)b );
}

void memcheck(void* pointer){
	if(pointer == 0){
		printf("Out of memeory\n");
		exit(0);
	}
	return;
}


DBL_WORD find_max_doc(FILE* file){

	char ch;
	DBL_WORD count,max,noofdocs=0;
	while(!feof(file)){
		ch = fgetc(file);
		if(ch!='\n')
			count++;
		else{
			if(count > max)
				max = count;
			count = 0;
			noofdocs++;
		}

	}
	if(count > max)
		max = count;

	printf("%ld\n",noofdocs );
	return max;
}

int relavence(int querylen, int title_count,int content_count, int longest_substring, int count_of_longest_substr, int content_count3){
	
	int temp = 0;

	if(longest_substring > 2)
		temp = ((int)((longest_substring*1)/(double)querylen));	
	
	return title_count*1000 + 50*content_count + 50*content_count3 + temp*count_of_longest_substr;
}

int main(int argc, char* argv[]){
	
	time_t t; 
	srand((unsigned)time(&t));
	struct timespec start,a;
	struct timespec end,b;
	double time_tree_creation=0, time_query=0,time=0;

	SUFFIX_TREE* tree;
	DBL_WORD len = 0,count=0;

	int doc_no=1;
	int title_count=0,content_count=0,max=0,match_count=0,queue_head=0,total_list=0;
	
	FILE* file = 0;
	file = fopen("Data/clean_data_cs.csv","r");
	
	if(file == 0){
			printf("can't open file.\n");
			return 0;
		}


	fseek(file, 0, SEEK_END);
	len = ftell(file);			//1,255,876,492
	fseek(file, 0, SEEK_SET);

	//printf("%ld\n",len);
	
	//DBL_WORD max_doc_len = find_max_doc(file); //482482, takes about 2 mins 

	unsigned char* line = (unsigned char*)malloc(sizeof(unsigned char)*MAX_DOC_SIZE);
	memcheck(line);
	
	printf("Data Source Ready\n");

	printf("Enter Query String\n");
	fflush(stdin);
	char* query = (char*)malloc(sizeof(char)*MAX_QUERY_LEN);
	memcheck(query);
	scanf("%[^\n]",query);

	printf("\nHow many documnets to Search?\n");
	int DOC_LIMIT;
	scanf("%d",&DOC_LIMIT);
	printf("\nEnter number of documents to be returned: \n");
	int k;
	scanf("%d",&k);

	int* doc = (int*)malloc(sizeof(int)*k);
	memcheck(doc);
	int* points = (int*)malloc(sizeof(int)*k);
	memcheck(points);
	unsigned char** return_doc = (unsigned char**)malloc(sizeof(unsigned char*)*k);
	memcheck(return_doc);
	char** return_doc_title    = (char**)malloc(sizeof(char*)*k);
	memcheck(return_doc_title);

	for (int i = 0; i < k; ++i)
	{
		return_doc[i] 		= (unsigned char*)malloc(sizeof(unsigned char)*MAX_DOC_SIZE);
		return_doc_title[i] = (char*)malloc(sizeof(char)*100);
		memcheck(return_doc[i]);
		memcheck(return_doc_title[i]);
	}

	printf("Run Case Sensitive Queries? (y/n) ");
	fflush(stdin);
	scanf(" %c",&choice);
	printf("\n");

	clock_gettime(CLOCK_REALTIME, &a);

	if(choice == 'n'){
		strlwr(query);
	}

	char* word,*title,*content;
	
	int rank=0;

	int index[MAX_DOC_SIZE];

	printf("First Occurence in Every document\n");

	while(!feof(file)){
		title_count=0;
		fgets(line,MAX_DOC_SIZE,file);

		if(choice == 'n')
			strlwr(line);

		title   = strtok(line,",");
		content = strtok(NULL,"");
		
		char* temp = strstr(title,query);
		if(temp != NULL)
			title_count = 1;

		clock_gettime(CLOCK_REALTIME, &start);
		tree = ST_CreateTree(content,strlen(content));
		clock_gettime(CLOCK_REALTIME, &end);

		time_tree_creation += time_elapsed(&start, &end);
		
		clock_gettime(CLOCK_REALTIME, &start);

		content_count = ST_FindSubstring(tree, (unsigned char*)query, strlen(query),index,0);
		

		int local_max =0, local_index=0,local_count = 1,content_count2=0,q_index;
		if((content_count == 0) && (title_count == 0)){

			while(local_count<strlen(query)){

				content_count2 = ST_FindSubstring(tree, (unsigned char*)(&query[local_count]),strlen(query)-local_count,index,1);

				if(content_count2 > local_max){
					local_max   = content_count2;
					local_index = index[0];
					q_index 	= local_count;
				}
				local_count++;
			}

			char* new_query = (char*)malloc(sizeof(char)*local_max);
			int i;
			for (i = 0; i < local_max; ++i)
			{  
				new_query[i] = query[i+q_index];
			}
			new_query[i]='\0';

			content_count2 = ST_FindSubstring(tree, (unsigned char*)(new_query),strlen(new_query),index,0);
			printf("Doc %d longest substring ->%d<-\t  count: %d\n",doc_no,local_index, content_count2 );
			if(strlen(new_query)<3)
				content_count2 = 0;
			free(new_query);
		}
		else{
			printf("Doc %d %d \n",doc_no,index[0] );
		}

		int content_count3=0;
	
		word   = strtok(query," ");
				
		while(word!=NULL){
			
				if(strlen(word)>2)
					content_count3 += ST_FindSubstring(tree, (unsigned char*)word, strlen(word),index,0);
			
				word = strtok(NULL," ");

		} 

		//}

		rank = relavence(strlen(query),title_count,content_count,local_max,content_count2,content_count3);
		
		if(rank >= max){
			max  = rank;
			if(match_count >= k){
				match_count=0;
			}
			queue_head = match_count;
			points[match_count] = rank;
			doc[match_count] = doc_no; 
			strcpy(return_doc[match_count],content);
			strcpy(return_doc_title[match_count],title);
			match_count++;
			total_list++;
		}

		clock_gettime(CLOCK_REALTIME, &end);
		time_query += time_elapsed(&start,&end);

		
		strcpy(line,"");
		doc_no++;

		ST_DeleteTree(tree);
		if(doc_no>DOC_LIMIT)
			break;
	}

	//printf("\033[1;34m");
	printf("\n********************************************************\n");
	printf("\nSearch Results in order of relavence:\n");
	int k2 = k;
	while(total_list>0 && k2>0 ){
		
		if(queue_head<0)
			queue_head = k2-1;
		printf("\nDoc no: %d\t -> %s (%d) \n",doc[queue_head],return_doc_title[queue_head],points[queue_head] );
		queue_head--;
		k2--;
		match_count--;
		total_list--;
	}
	//printf("\033[0m");

	printf("\nTree construction time %lf\n",time_tree_creation );
	printf("Query Search Time %lf\n",time_query );
	printf("\n");
	clock_gettime(CLOCK_REALTIME, &b);
	time = time_elapsed(&a,&b);
	printf("Time for the whole process : %lf\n",time );

	while(1){
		printf("\n");
		printf("Enter 0 to quit.\n");
		printf("Enter doc no, to view the document. \n");

		int ch;
		fflush(stdin);
		scanf(" %d",&ch);
		if(ch==0)
			break;
		else{
			int i;
			for (i = 0; i < k; ++i)
			{
				if(doc[i] == ch){
					printf("%s\n",return_doc[i] );
					break;
					}
				
			}
			if(i==k)
				printf("Wrong doc no.!\n");
		}
	
	}

	
	fclose(file);
	free(line);
	free(query);
	free(doc);
	free(points);

	for (int i = 0; i < k; ++i)
	{
	free(return_doc_title[i]);
	free(return_doc[i]);
	}
	free(return_doc_title);
	free(return_doc);
	


	printf("\nDone!\n");
	return 0;
}