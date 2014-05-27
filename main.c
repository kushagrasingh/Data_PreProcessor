#include<stdio.h>
#include<stdlib.h>
#include<string.h>

/* Data Mining - course assignment */

typedef char string[50];

typedef struct
{
    string attr_name;
    string attr_type;
    int num_values;
    string* attr_values;

} attribute;


int count = 0;
int num_attr;
int num_inst;
char c;
attribute* attr;
int i,j;
FILE *fp;
string table[60][20];
int missing[20];


void print_attset_properties()
{
    printf("\nThe attributes and their properties are as follows : \n");
    printf("\n(Name)\t\t\t(Type)\t\t(Values-it-can-have)\n");
    printf("-------------------------------------------------------------------\n");

    int num_real = 0;
    int num_nominal = 0;
    char at_name[50];
    char at_type[10];

    for(i=0;i<num_attr;i++)
    {
        if(!strcmp(attr[i].attr_type,"real"))
        {
            num_real++;
        }
        else num_nominal++;
    }

    printf("\nThe number of real attributes is : %d\n",num_real);
    printf("\nThe number of nominal attributes excluding the class attribute is: %d\n\n",--num_nominal);
    for(i=0; i<50; i++)   at_name[i] = ' ';
    for(i=0; i<num_attr; i++)
    {
        strcpy(at_name,attr[i].attr_name);
        strcpy(at_type,attr[i].attr_type);

        printf("%s\t%s",at_name,at_type);


        if(attr[i].num_values == 1) printf("\tNA");

        else
        {
            printf("\t");
            for(j=0; j<attr[i].num_values; j++)
                printf("%s,",attr[i].attr_values[j]);
        }

        printf("\n\n");
    }


    /*for(i=0;i<num_attr;i++)
    {
    if(strlen(attr[i].attr_name) > 10 && strlen(attr[i].attr_name) < 15 )
        printf("%s\t\t\t\t\t%s",attr[i].attr_name,attr[i].attr_type);
    else if(strlen(attr[i].attr_name) > 15)
        printf("%s\t\t\t%s",attr[i].attr_name,attr[i].attr_type);
    else printf("%s\t\t\t\t\t%s",attr[i].attr_name,attr[i].attr_type);
    if(attr[i].num_values == 1) printf("\t\tNA");

    else
    {
        printf("\t\t");
        for(j=0;j<attr[i].num_values;j++)
            printf("%s,",attr[i].attr_values[j]);
    }

    printf("\n\n");
    }*/
}

void createAttSet()
{
    for(i=0; i<num_attr; i++)
    {
        attr[i].num_values = 0;
        attr[i].attr_values = NULL;
    }

    for(i=0; i<num_attr; i++)
    {
        fscanf(fp,"%s %s %d",attr[i].attr_name,attr[i].attr_type,&attr[i].num_values);

        if(!strcmp(attr[i].attr_type,"nominal"))
        {
            attr[i].attr_values = (string*)malloc( attr[i].num_values * sizeof(string));

            for(j=0; j<attr[i].num_values; j++)
                fscanf(fp,"%s",attr[i].attr_values[j]);
        }

    }
}


void populateTuples()
{
    for(i=0; i<num_inst; i++)
    {

        for(j=0; j<num_attr; j++)
        {

            fscanf(fp,"%s ",table[i][j]);
        }

    }
}




void printTupleTable()
{
    printf("\n");

    printf("\n----------------------------------------------------------\n");

    printf("Following are the data tuples in this data set\n\n\n");

    for(i=0; i<num_inst; i++)
    {
        for(j=0; j<num_attr; j++)
        {
            printf("%s ",table[i][j]);
        }

        printf("\n");
    }
}

void countMissingValues()
{
    for(i=0; i<num_inst; i++)
    {
        for(j=0; j<num_attr; j++)
        {
            if(!strcmp(table[i][j],"?"))
            {
                count++;
                missing[j]++;
            }

        }
    }

    printf("\n\nThere are %d missing values in this data set\n\n",count);
}

void listMissingValuesbyAttribute()
{
    for(i=0; i<num_attr; i++)
    {
        printf("\nAttribute %s has %d missing values\n",attr[i].attr_name,missing[i]);
    }
}


void replaceMissingValuesReal(int i)
{
    float res= 0;
    int k;
    for(k=0; k<num_inst; k++)
    {
        if(strcmp(table[k][i],"?") != 0)
            res += strtof(table[k][i],NULL);  //strtof converts string to float

    }

    res = res/num_inst;

    //res = truncf(res);
    //printf("%.2f\n",res);

    for(k=0; k<num_inst; k++)
    {
        if(!strcmp(table[k][i],"?"))
        {
            sprintf(table[k][i],"%.2f",res);
        }
    }

}

void replaceMissingValuesNominal(int i)
{
    int num = attr[i].num_values;
    int max,k;
    int nums[5]= {0,0,0,0,0};

    if(num > 1)
    {


        for(j=0; j<num_inst; j++)
        {
            for(k=0; k<num; k++)
            {
                if(!strcmp(table[j][i],attr[i].attr_values[k]))
                    nums[k]++;
            }
        }


        max = nums[0];
        int index = 0;
        for(j=0; j<num; j++)
        {
            if(nums[j] > max)
            {
                max = nums[j];
                index = j;
            }
        }

        string res;
        strcpy(res,attr[i].attr_values[index]);

        for(j=0; j<num_inst; j++)
        {
            if(!strcmp(table[j][i],"?"))
                strcpy(table[j][i],res);

        }

    }

}
void replaceMissingValues()
{
    float res = 0;
    for(i=0; i<num_attr; i++)
    {
        if(!strcmp(attr[i].attr_type,"real"))
        {
            replaceMissingValuesReal(i);
        }

        else
        {
            if(!strcmp(attr[i].attr_type,"nominal"))
            replaceMissingValuesNominal(i);
        }
    }

}


int areDuplicate(int m, int n)
{
    for(i=0;i<num_attr;i++)
    {
        if(strcmp(table[m][i],table[n][i]) != 0) return 0;
    }

    return 1;
}

//int areDuplicate(int m,int n)
//{
 //  return compareTuples(m,n);
//}

void deDuplicate()
{
    int k=0,m;
    for(i=0;i<num_inst;i++)
    {
        for(j=0;j<num_inst;j++)
        {
            if(i != j && areDuplicate(i,j))
            {
                for(k=(num_inst-1);k>i;k--)
                {
                    for(m=0;m<num_attr;m++)
                    {
                        strcpy(table[k-1][m],table[k][m]);
                    }
                }
            }
        }
    }
}

void checkDuplicates()
{
    int num = 0,k=0;
    for(i=0;i<num_inst;i++)
    {
        for(k=0;k<num_inst && k != i;k++)
        {
          if(areDuplicate(i,k))
              num++;
        }
    }

    printf("\n\nThere are %d duplicate tuples in this data set\n\n",num);

    //if(num > 0) deDuplicate();
}



float findMax(int j)
{
    float res = strtof(table[0][j],NULL);


    for(i=1;i<num_inst;i++)
    {
        float val = strtof(table[i][j],NULL);
        if(res > val)
            res = val;
    }

    return res;
}


float findMin(int j)
{
    float res = strtof(table[0][j],NULL);
    for(i=1;i<num_inst;i++)
    {
        float val = strtof(table[i][j],NULL);
        if(res < val)
            res = val;
    }

    return res;
}


void normalize(int j)
{
    printf("\n\n Normalize attribute %s to values between 0 and 1\n\n",attr[j].attr_name);
    float max = findMax(j);
    float min = findMin(j);

    float diff = max - min;
    float val;
    float res;
    for(i=0;i<num_inst;i++)
    {
        val = strtof(table[i][j],NULL);
        res = (val - min)/diff;
        sprintf(table[i][j],"%.2f",res);
    }

}

void normalizeData()
{
    for(i=0;i<num_attr;i++)
    {
        if(!strcmp(attr[i].attr_type,"real"))

            normalize(i);
    }
}


int main(void)
{
    fp = fopen("data","r");

    if(fp == NULL)
        printf("Could not open file!!\n");

    printf("\nData Set Information:\n");
    getchar();

    fscanf(fp,"%d",&num_attr);
    printf("The number of attributes in this data set is %d\n",num_attr);

    printf("\n");

    fscanf(fp,"%d",&num_inst);
    printf("The number of tuples in this data set is %d\n",num_inst);

    num_attr++;

    attr = (attribute*)malloc(num_attr * sizeof(attribute));
    if(attr == NULL) printf("\nCould not allocate memory for attributes!!\n");

    createAttSet();

    getchar();
    print_attset_properties();

    string s1;
    fscanf(fp,"%s",s1);
    //printf("%s",s1);

    getchar();
    populateTuples();
    printTupleTable();

    printf("\nReplacing missing values: Press any key to continue...\n");
    c = getchar();
    countMissingValues();
    listMissingValuesbyAttribute();

    printf("\n\nPress any key:\n\n");
    replaceMissingValues();
    printf("\n\nAfter Replacing Missing Values: \n\n");
    printTupleTable();



   // checkDuplicates();

    //printf("\n\nHERE\n\n");
    normalizeData();
    //printf("\n\n Normalizing Data in this table\n\n:Press any key to o");
    //getchar();
    printTupleTable();

    return 0;
}
