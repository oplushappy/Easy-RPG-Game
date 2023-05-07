#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<conio.h>
#include<windows.h>
int**create_map(int*row,int*column);
int check_boundary(int p_row,int p_column,int row,int column);
int check_availability(int p_row,int p_column,int row,int column,int**map);
void monster_drop(int row,int column,int**map);
void boss_drop(int row,int column,int**map);
void setup_player();
void setup_village(int row,int column,int**map);
void trap(int row,int column,int**map);
void trap_decrease_blood();
void weapon_drop(int row,int column,int**map);
void judge_win(int**map,int row,int column);

int check_move(int p_row,int p_column,int row,int column,int**map);
void go_up(int*p_row,int*p_column,int**map,int row,int column);
void go_down(int*p_row,int*p_column,int**map,int row,int column);
void go_left(int*p_row,int*p_column,int**map,int row,int column);
void go_right(int*p_row,int*p_column,int**map,int row,int column);

void do_village();
void choice_enter_village();
void mall();
void bag_judge();
void gun();
void sword();
int*weapon_quip_index();
int*weapon_type_f();

int monster_level_f();
int monster_blood_f(int monster_level);
int monster_attack_f(int monster_level);
int monster_defend_f(int monster_level);
int monster_drop_experience(int monster_level); 
int monster_drop_money(int monster_level);

int*player_level_f();
int*player_strength();
int*palyer_origin_attack();
int*player_defend_f();
int*player_blood_f();
int*player_max_blood_f();
int*money();
int*player_experience();

void look_level();
void level_up();

int attack(int p_row,int p_column,int**map,int judge_boss);
int dodge();

int*bag_f();
int*bag_index_f();
int*player_weapon_equip_judge();
void look_bag();
void change_weapon();
void add_warrior(int warrior);
void Weapon();
void Medicinal_liquid();
void use_medicine();



int**create_map(int*row,int*column)
{
    printf("start game\n");
    printf("========\n");
    printf("Input the number of row and column for the map:\n");

    int m,n;
    scanf("%d%d",&m,&n);//player input row,column
    *row=m;
    *column=n;
    int**map=(int**)malloc((*row)*sizeof(int*));//**map save the address of each row 
    for(int i=0;i<(*row);i++)
    {
        map[i]=(int*)malloc((*column)*sizeof(int));//map[i] is the each row save each column 
    }
    for(int i=0;i<*row;i++)
    {
        for(int j=0;j<*column;j++)
        {
            map[i][j]='.';
        }
    }

    boss_drop(*row,*column,map);
    setup_village(*row,*column,map);
    weapon_drop(*row,*column,map);
    monster_drop(*row,*column,map);
    trap(*row,*column,map);

    return map;
}
void boss_drop(int row,int column,int**map)
{
    srand((unsigned)time(NULL));
    int monster_number = rand()%2+1;//monster number = boss number = 1~2

    for(int i=0;i<monster_number;)
    {
        int drop_row=rand()%row;
        int drop_column=rand()%column;
        if(map[drop_row][drop_column] == '.')
        {
            map[drop_row][drop_column]= 'B';
            i++;
        }    
    }
}

void print_map(int**map,int row,int column)
{
    judge_win(map,row,column);
    printf("=== MAP ===\n");
    for(int i=0;i<row;i++)
    {
        for(int j=0;j<column;j++)
        {
            if(map[i][j]=='t' || map[i][j]=='s') //not show the trap
            {
                printf(". ");
            }
            else
            {
                printf("%c ",map[i][j]);
            }
            
        }
        printf("\n");
    }
    printf("=== MAP ===\n");
}
void judge_win(int**map,int row,int column)
{
    int number=0; //number is the number of monster still survived in the map
    for(int i=0;i<row;i++)
    {
        for(int j=0;j<column;j++)
        {
            if(map[i][j]=='M' || map[i][j]=='B')
            {  
                number++;
            }    
        }
    }
    if(number==0)
    {
        printf("You win");
        for(int i=0;i<row;i++)
        {
            free(map[i]);
        }
        free(map);
        exit(0);
    }
}
void weapon_drop(int row,int column,int**map)
{
    int weapon_number = (int)(row*column*0.02);//(row/5) + (row % 5)
    srand((unsigned)time(NULL));
    for(int i=0;i<weapon_number;)
    {
        int drop_row=rand()%row;
        int drop_column=rand()%column;
        if(map[drop_row][drop_column] == '.')
        {
            map[drop_row][drop_column]= 's';
            i++;
        }    
    }
}
void monster_drop(int row,int column,int**map)
{
    int monster_number = 2*row;//(row/10) + (row % 10);
    srand((unsigned)time(NULL));
    for(int i=0;i<monster_number;)
    {
        int drop_row=rand()%row;
        int drop_column=rand()%column;
        if(map[drop_row][drop_column] == '.')
        {
            map[drop_row][drop_column]= 'M';
            i++;
        }    
    }
}
void setup_village(int row,int column,int**map)
{
    int drop_row=rand()%row;
    int drop_column=rand()%column;
    map[drop_row][drop_column]='v';  
}
void trap(int row,int column,int**map)
{
    int trap_number = (row/10) + (row % 10);
    srand((unsigned)time(NULL));
    for(int i=0;i<trap_number;)
    {
        int drop_row=rand()%row;
        int drop_column=rand()%column;
        if(map[drop_row][drop_column] == '.')
        {
            map[drop_row][drop_column]= 't';
            i++;
        }    
    }
}
void setup_player(int row,int column,int**map,int*p_row,int*p_column)
{ 
    int judge1=0,judge2=0;//judge receive the return value 
    while(judge1!=1 ||  judge2!=1)
    {
        printf("Input the number of row and column for player:\n");
        scanf("%d %d",p_row,p_column);
        judge1=check_boundary(*p_row,*p_column,row,column);
        judge2=check_availability(*p_row,*p_column,row,column,map);
        if(judge1==1 && judge2==1)
        {
            map[*p_row][*p_column]='p';
        }
    }
}
int check_boundary(int p_row,int p_column,int row,int column)
{                              
    if(p_row>=row || p_column>=column)
    {    
        printf("the location is outside the map\n");
        return 0;
    }
    else if(p_row<0 || p_column<0)
    {
        printf("wrong input , please input >= 0 \n");
        return 0;
    }
    else
        return 1;
}
int check_availability(int p_row,int p_column,int row,int column,int**map)
{
    if(p_row<row && p_column<column && p_row>=0 && p_column>=0)//make sure the value of row and column is right
    {
        if(map[p_row][p_column]!='.')
        {
            printf("the location is occupied\n");
            return 0;
        } 
        else    
            return 1; 
    }
    else 
        return 1;

    
}
int console()
{ 
    char input=_getch();//when press keyboard , the symbol will transfer into ascii to save
    return input;
}
void player_move(int*p_row,int*p_column,int**map,int row,int column)
{
    switch(console())
    {
        case 72:go_up(p_row,p_column,map,row,column);break;//up
        case 80:go_down(p_row,p_column,map,row,column);break;//down
        case 75:go_left(p_row,p_column,map,row,column);break;//left
        case 77:go_right(p_row,p_column,map,row,column);break;//right
        case 'b':
            look_bag();
            printf("press to continue\n");
            break;//break will go back to main function,and then continue to run
        case 'u':
            level_up();
            printf("press to continue\n");
            break;
        case 'l':
            look_level();
            printf("press to continue\n");
            break;
    }
}
int check_move(int p_row,int p_column,int row,int column,int**map)
{
    if(p_row<0 || p_column<0 || p_row>=row || p_column>=column )
    {
        printf("you can't go there\n");
        return 0;
    }
    else if(map[p_row][p_column]=='M' )//encounter monster
    {
        if(attack(p_row,p_column,map,0)==1)
        {
            map[p_row][p_column]='.';
        }
        print_map(map,row,column);
        return 0;    
    }
    else if(map[p_row][p_column]=='v' )//encounter village
    {
        choice_enter_village();
        print_map(map,row,column);
        return 0;
    }
    else if(map[p_row][p_column]=='t' )//encounter trap
    {
        trap_decrease_blood();
        print_map(map,row,column);
        return 0;
    }
    else if(map[p_row][p_column]=='s')//encounter swordw
    {
        printf("you get the sword\n");
        add_warrior(map[p_row][p_column]);
        map[p_row][p_column]='.';
        print_map(map,row,column);
        return 0; 
    }
    else if(map[p_row][p_column]=='B' )//encounter boss
    {
        if(attack(p_row,p_column,map,1)==1)
        {
            map[p_row][p_column]='.';
        }
        print_map(map,row,column);
        return 0;    
    }
    else
    { 
        map[p_row][p_column]='p';
        return 1;    
    }
}
void add_warrior(int warrior)
{
    int*bag=bag_f();//bag as a pointer save the address of bag[] array
    int*bag_index=bag_index_f();//bag_index as a pointer save the address of (the last saving thing array index+1) 
    bag[*bag_index]=warrior;
    *bag_index+=1;
}
void trap_decrease_blood()
{
    int*player_blood=player_blood_f(); 
    *player_blood-=1;
    printf("you encounters with a trap\n");
    printf("your blood is %d\n",*player_blood);

}
void go_up(int*p_row,int*p_column,int**map,int row,int column)
{

    int origin_row=*p_row;
    (*p_row)-=1;;
    if(check_move(*p_row,*p_column,row,column,map)==1)
    {   
        map[origin_row][*p_column]='.';
        system("cls");//clean the screen
        print_map(map,row,column);
    }
    else
        *p_row+=1;
} 
void go_down(int*p_row,int*p_column,int**map,int row,int column)
{
    int origin_row=*p_row;
    (*p_row)+=1;;
    if(check_move(*p_row,*p_column,row,column,map)==1)
    {   
        map[origin_row][*p_column]='.';
        system("cls");
        print_map(map,row,column);
    }
    else
        *p_row-=1;
}
void go_left(int*p_row,int*p_column,int**map,int row,int column)
{
    int origin_column=*p_column;
    *p_column-=1;
    if(check_move(*p_row,*p_column,row,column,map)==1)
    {   
        system("cls");
        map[*p_row][origin_column]='.';
        print_map(map,row,column);
    }
    else
        *p_column+=1;
}
void go_right(int*p_row,int*p_column,int**map,int row,int column)
{
    int origin_column=*p_column;
    *p_column+=1;
    if(check_move(*p_row,*p_column,row,column,map)==1)
    {   
        map[*p_row][origin_column]='.';
        system("cls");
        print_map(map,row,column);
    }
    else
        *p_column-=1;
}
int*player_blood_f()
{
    static int blood=150;
    return &blood;
}
int*player_max_blood_f()
{
    static int blood=150;
    return &blood;
}
int*player_strength()
{
    static int attack=10;
    return &attack;
}
int*palyer_origin_attack()
{
    static int attack=10;
    return &attack;
}
int*player_defend_f()
{
    static int player_defend=10;
    return &player_defend; 
}
int*player_experience()
{
    static int player_exp=0;
    return &player_exp;
}
int*player_level_f()
{
    static int level=1;
    return &level; 
}
void look_level()
{
    int*player_level=player_level_f();
    int*player_exp=player_experience();
    int*player_blood=player_blood_f();
    int*player_attack=player_strength();
    int*player_defend=player_defend_f();
    int*player_money=money();
    printf("============= Value ==========\n");
    printf("player level: %d \n",*player_level);
    printf("player attack: %d \n",*player_attack);
    printf("player defend: %d \n",*player_defend);
    printf("player blood:%d \n",*player_blood);
    printf("player exp: %d\n",*player_exp);
    printf("level up need exp: %d\n",(*player_level*500));
    printf("player money:%d \n",*player_money);
    printf("==============================\n");
}
void level_up()
{
    int*player_blood=player_blood_f();
    int*player_attack=player_strength();
    int*player_defend=player_defend_f();
    int*player_level=player_level_f();
    int*player_exp=player_experience();
    int*player_max_blood=player_max_blood_f();
    int*player_o_attack=palyer_origin_attack();
    int level_choice;
    if(*player_exp>(*player_level*500))
    {
        printf("Make sure you want to level up(1: Yes; 0: No)");
        scanf("%d",&level_choice);
        if(level_choice==1)
        {
            *player_exp-=(*player_level*500);
            *player_level+=1;
        }
        printf("You level up successfully,press L to check your Ability value\n");
        *player_blood+=(*player_level)*7;//level up ,value of ability will upgrade
        *player_max_blood+=(*player_level)*7;

        *player_attack+=(*player_level)*6;
        *player_o_attack+=(*player_level)*6;

        *player_defend+=(*player_level)*5;
    }
    else 
    {
        printf("your exp is not enough\n");
    }
}
int monster_defend_f(int monster_level)//when encounter a monster each time ,will give monster a new value of ability ,therefore  we do not use pointer
{
    int monster_deffend=10 + monster_level*3;
    return monster_deffend;
}
int monster_drop_experience(int monster_level)
{
    int exp=monster_level*200;
    return exp;
}
int monster_drop_money(int monster_level)
{
    int money=monster_level*50;
    return money;
}
int monster_level_f()
{
    int*player_level=player_level_f();
    static int level=0;
    level=(*player_level)+rand()%5;//monster level will be higher than player 0~5 level
    return level; 
}
int monster_blood_f(int monster_level)
{
    
    int monster_blood=100 + monster_level*3;
    return monster_blood;
}
int monster_attack_f(int monster_level)
{
    
    int monster_attack=20 + monster_level*3;
    return monster_attack;
}
/*---------------------------------*/
int boss_defend_f(int boss_level)
{
    int boss_deffend=10 + boss_level*10;
    return boss_deffend;
}
int boss_blood_f(int boss_level)
{
    
    int boss_blood=100 + boss_level*12;
    return boss_blood;
}
int boss_attack_f(int boss_level)
{
    
    int boss_attack=20 + boss_level*10;
    return boss_attack;
}
int dodge()
{
    srand((unsigned)time(NULL));
    int rst= rand()%100;//get a number between 0~99
    if(rst<25)
    {
        printf("Defender dodge successfully\n");
        return 1;
    }
    else 
    {
        printf("Defender dodge fail\n");
        return 0;
    }
        
}
void look_bag()
{
    int*bag=bag_f();
    printf("You can put 25 things on bag\n");
    for(int i=0;i<25;i++)
    {
        printf("%c ",bag[i]);
    }
    printf("\n");
    int choice_bag;
    do{
        printf("Do you want to equip any weapon (1: Yes; 0: No)? ");
        scanf("%d",&choice_bag);
        if(choice_bag==1)
        {
            bag_judge();
        }
    }while(choice_bag!=1 && choice_bag!=0);
}
int attack(int p_row,int p_column,int**map,int judge_boss)
{
    int*player_blood=player_blood_f();
    int*player_attack=player_strength();
    int*player_defend=player_defend_f();
    int*player_level=player_level_f();
    int*player_money=money();
    int*player_exp=player_experience();

    int monster_level=monster_level_f();
    int monster_blood,monster_attack,monster_defend;
    if(judge_boss==0)//judge a monster or a boss
    {
        monster_blood=monster_blood_f(monster_level);
        monster_attack=monster_attack_f(monster_level);
        monster_defend=monster_defend_f(monster_level);
    }
    else 
    {
        monster_blood=boss_blood_f(monster_level);
        monster_attack=boss_attack_f(monster_level);
        monster_defend=boss_defend_f(monster_level);
    }
    int hurt;//monster give player or player give monster 
    int turn=1;
    while(*player_blood>0 && monster_blood>0)
    {    
        if(turn==1)
        {
            printf("Your Level: %d\n",*player_level);
            printf("Monster Level: %d\n",monster_level);
            printf("Monster defend: %d\n",monster_defend);
            printf("Monster basic attack: %d\n",monster_attack);
            printf("================================\n");
        }   
        if(turn%2==1)
        {
            printf("Turn:%d\n",turn);
            printf("You have %d blood and %d attack.\n",*player_blood,*player_attack);
            
            switch(console())
            {
                case 'a':
                    printf("you attack monster\n");
                    Sleep(1000);
                    srand((unsigned)time(NULL));
                    int player_attack_up=(int)((*player_attack)*(rand()/(RAND_MAX+1.0)+1));
                    int dodge_activity=dodge();
                    if(player_attack_up >= monster_defend && dodge_activity==0)
                    {
                        hurt=player_attack_up-monster_defend;
                        monster_blood-=hurt;
                        printf("You hurt monster %d blood\n",hurt);
                        printf("Monster have %d blood.\n",monster_blood);
                    }
                    else if(player_attack_up < monster_defend && dodge_activity==0)
                    {
                        printf("You hurt monster 0 blood.\n");
                        printf("Monster have %d blood.\n",monster_blood);
                    }
                    else if(dodge_activity==0){}
                    printf("================================\n");
                    break;
                case 'e':
                    monster_blood=-1000;
                    break;
                case 'b':
                    look_bag();
                    break;
            }
        }
        else if(turn%2==0)
        {
            printf("Turn:%d\n",turn);
            printf(" Monster attack you\n");
            Sleep(1000);
            int dodge_activity=dodge();
            if(dodge_activity==0)
            {
                srand((unsigned)time(NULL));
                int monster_attack_up=(int)((monster_attack)*(rand()/(RAND_MAX+1.0)+1));
                hurt=monster_attack_up - *player_defend;
                if(hurt>0)
                    *player_blood-=hurt; 
                printf("monster hurt you %d.\n",hurt);  
            }      
            printf("You have %d blood.\n",*player_blood);
            printf("================================\n");
               
        }
        turn++;    
    }
    if(*player_blood>0 && monster_blood!=(-1000))
    {
        printf("you win\n");
        int get_money=monster_drop_money(monster_level);
        int get_exp=monster_drop_experience(monster_level);
        *player_money+=get_money;
        *player_exp+=get_exp;
        printf("you get %d money\n",get_money);
        printf("you get %d exp\n",get_exp);
        return 1;
    }
    else if(*player_blood<=0)
    {
        printf("you lose\n");
        return 0;
    }
    else if(monster_blood==(-1000))
    {
        printf("you flee\n");
        return 0;
    }
    else
        return 0;
}
int*money()
{
    static int money=500;
    return &money;
}
int*bag_f()
{
    static int bag[50];
    return bag;
}
int*bag_index_f()
{
    static int bag_index=0;
    return &bag_index;
}
void Weapon()
{
    int*player_money=money();
    int*player_origin_attack=player_strength();
    int*bag=bag_f();
    int weapon_choice;
    printf("You have %d money.\n",*player_money);
    int*i=bag_index_f();
    do{
        printf("1.sword(500) player_attack*1.25\n");
        printf("2.gun (1000) player_attack*1.75\n");
        printf("3.Back :");
        scanf("%d",&weapon_choice);
        switch(weapon_choice)
        {
            case 1:
                if(*player_money >= 500)
                {
                    (*player_money)-=500;
                    add_warrior('s');
                    printf("You have buy the sword\n");
                    printf("You have %d money now\n",*player_money);
                }
                break;
            case 2:
                if(*player_money >= 1000)
                {
                    (*player_money)-=1000;
                    add_warrior('g');
                    printf("You have buy the gun\n");
                    printf("You have %d money now\n",*player_money);
                }
                break;
            case 3:
                break;
        }
    }while(weapon_choice>3 || weapon_choice<=0);
}
void Medicinal_liquid()
{
    int*player_money=money();
    int*player_blood=player_blood_f();
    int*bag=bag_f();
    int medice_choice;
    printf("You have %d money.\n",*player_money);
    int*i=bag_index_f();
    do{
         printf("1.low level potion(50) ; 2.middle Level Potion(150) ; 3.high Level Potion(300) ; 4.back :");
        scanf("%d",&medice_choice);
        switch(medice_choice)
        {
            case 1:
                if(*player_money >= 50)
                {
                    (*player_money)-=50;
                    add_warrior('l');
                    printf("You have buy the low position\n");
                    printf("You have %d money now\n",*player_money);
                }
                break;
            case 2:
                if(*player_money >= 200)
                {
                    (*player_money)-=200;
                    add_warrior('m');
                    printf("You have buy the middle position\n");
                    printf("You have %d money now\n",*player_money);
                }
                break;
            case 3:
                if(*player_money >= 300)
                {
                    (*player_money)-=300;
                    add_warrior('h');
                    printf("You have buy the high position\n");
                    printf("You have %d money now\n",*player_money);
                }
                break;
            case 4:
                break;
        }
    }while(medice_choice>4 || medice_choice<=0);
}
void mall()
{
    int mall_choice;
    do
    {
        printf("1.Weapon ; 2.Medicinal liquid ; 3.Back :");
        scanf("%d",&mall_choice);
        switch(mall_choice)
        {
            case 1: Weapon();break;
            case 2: Medicinal_liquid();break;
            case 3: break;   
        }    
    }while(mall_choice>3 ||  mall_choice<=0);
        
}
void do_village()
{
    int*player_attack=player_strength();
    int*player_blood=player_blood_f();
    int*player_max_blood=player_max_blood_f();
    int*bag=bag_f();
    int choice_do_village;
    do{
        printf("What do you want to do in our village"
        "(1: go to mall; 2: go to the bar; 3: leave the village; 4:look your bag?): ");
        scanf("%d",&choice_do_village);
        if(choice_do_village==1)
        {
            mall();
        }
        else if(choice_do_village==2)
        {               
                        
            if(*player_blood>=*player_max_blood)
            {
                printf("Your blood is full. You don't need a beer.\n");
            }
            else
            {
                *player_blood = *player_max_blood;
                printf("Nice beer! Now you have %d blood!\n",*player_blood);
            }
        }
        else if(choice_do_village==3)
        {
            printf("You are welcome to come back anytime!\n");
        }
        else if(choice_do_village==4)
        {
            look_bag();
        }
    }while(choice_do_village!=3);
}
void bag_judge()
{
    int*bag=bag_f();
    int*equipped_number=player_weapon_equip_judge();
    int choice_weapon;
    do{
        printf("Do you want the weapon of what index? (begin with 1 ; back(0) ; remove weapon(26) )");
        scanf("%d",&choice_weapon);
        if(choice_weapon !=0 && choice_weapon >= 1 && choice_weapon <= 25)
        {
            choice_weapon--;
            if(bag[choice_weapon]=='g' && *equipped_number==0)
            {
                gun(choice_weapon);
                *equipped_number+=1;
            }
            else if(bag[choice_weapon]=='s' && *equipped_number==0)
            {
                sword(choice_weapon);
                *equipped_number+=1;
            }
            else if(bag[choice_weapon]=='l')
            {
                use_medicine(choice_weapon);
            }
            else if(bag[choice_weapon]=='m')
            {
                use_medicine(choice_weapon);
            }
            else if(bag[choice_weapon]=='h')
            {
                use_medicine(choice_weapon);
            }
        }
        else if(choice_weapon>26 || choice_weapon<0 )
        {
            printf("No such index,please input again\n");
        } 
        else if(*equipped_number==1 && (bag[choice_weapon]=='g' || bag[choice_weapon]=='s'))
        {
            printf("You have been equipped .\n");
        }
        else if(choice_weapon==26)
        {
            change_weapon();
        }   
    }while(choice_weapon!=0);
      
}
void use_medicine(int bag_index)
{
    int*player_blood=player_blood_f();
    int max_blood=*player_max_blood_f();;
    int*bag=bag_f();
   if(bag[bag_index]=='l')
   {
       *player_blood+=(max_blood/3);
       bag[bag_index]='.';
       printf("You use medicine , now you have %d blood\n",*player_blood);
   }
   else if(bag[bag_index]=='m')
   {
       *player_blood+=(max_blood/2);
       bag[bag_index]='.';
       printf("You use medicine , now you have %d blood\n",*player_blood);
   }
   else if(bag[bag_index]=='h')
   {
       *player_blood+=(max_blood);
       bag[bag_index]='.';
       printf("You use medicine , now you have %d blood\n",*player_blood);
   }
}
void gun(int bag_index)
{
    int*bag=bag_f();
    int*player_attack=player_strength();
    int*weapon_q_index=weapon_quip_index();//if we equip gun we will save its bag index in weapon_q_index
    int*wapon_type=weapon_type_f();//if we equip gun we will save its type index in wapon_type
    int gun=(int)((*player_attack)*2.5);//gun=attack*4.5
    *player_attack+=gun;
    printf("your attack after equipped:%d\n",*player_attack);
    bag[bag_index]='.';//make the bag do not have weapon in same index ,in order to recover weapon when player want to change weapon  
    *weapon_q_index=bag_index;
    *wapon_type='g';
    
}
void sword(int bag_index)
{
    int*bag=bag_f();
    int*player_attack=player_strength();
    int*weapon_q_index=weapon_quip_index();
    int*wapon_type=weapon_type_f();
    int sword=(int)((*player_attack)*2.2);
    *player_attack+=sword;
    printf("your attack after equipped:%d\n",*player_attack);
    bag[bag_index]='.';
    *weapon_q_index=bag_index;
    *wapon_type='s';
    
}
int*player_weapon_equip_judge()//player only can have only one weapon to equip at the same time 
{
    static int has_equip=0;
    return &has_equip;
}
int*weapon_type_f()
{
    static int weapon_type=0;
    return &weapon_type;
}
int*weapon_quip_index()
{
    static int quipped_index=0;
    return &quipped_index;
}
void change_weapon()
{
    int*bag=bag_f();
    int*player_attack=player_strength();
    int*player_o_attack=palyer_origin_attack();
    int*equipped_number=player_weapon_equip_judge();
    int*weapon_q_index=weapon_quip_index();
    int*wapon_type=weapon_type_f();
    int change_weapon_choice;
    do{
        printf("Do you want to remove weapon :(1: Yes; 0: No)? ");
        scanf("%d",&change_weapon_choice);
        if(change_weapon_choice ==1)
        {
            if(*equipped_number==1)//only have one weapon
            {
                *player_attack=*player_o_attack;
                printf("You have removed weapon.\n");
                printf("player attack: %d \n",*player_attack);
                bag[*weapon_q_index]=*wapon_type;
                *equipped_number-=1;
            }
            else
            {
                printf("You have not weapon to remove\n");
            }
            
        }
        
    }while(change_weapon_choice!=1  && change_weapon_choice!=0);
}

void choice_enter_village()
{
    int enter_village;
    do{
        printf("Do you want to go to our village (1: Yes; 0: No)? ");
        scanf("%d",&enter_village);
        if(enter_village==1)
        {
            do_village();
        }
        else if(enter_village==0){} 
    }while(enter_village!=1 && enter_village!=0);
}




int main()
{
    
    int row=0, column=0;
    int*p=&row;
    int*p2=&column;
    /*-----------------------*/
    int p_row=0,p_column=0;
    int*p3=&p_row;
    int*p4=&p_column;
    /*-----------------------*/

    int**map=create_map(p,p2);
    print_map(map,row,column);
    setup_player(row,column,map,p3,p4);
    print_map(map,row,column);
    while(console()!='e'){
        player_move(p3,p4,map,row,column);
    }
    return 0;
}