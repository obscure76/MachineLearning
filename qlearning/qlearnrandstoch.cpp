#include <qlearn.hpp>

template <typename T>
class matrix
{
    public:
    T **matr;
    int rows, cols;
    
    matrix(int srows, int scols)
    {
        rows = srows;
        cols = scols;
        matr = allocate_matrix();
        //cout<<rows<<' '<<cols<<endl;
    }
   
    ~matrix()
    {
        delete_matrix();
    }
    T** allocate_matrix()
    {
        T** temp;
        temp = new T* [rows];
        for(int i=0; i<rows; i++)
        {
            temp[i] = new T[cols];
        }
        return temp;
    }
    
    void init_matrix();
    void print_matrix();
    void delete_matrix();
    void init_row(int, int *);
    void init_row_random(int, int *);
    double maxi(int);
    int maxaction(int);
    double get_random_zo(void);
};

template <typename T>
void matrix<T>::delete_matrix()
{
    for(int i=0;i<rows;i++)
    {
        delete matr[i];
    }
    delete matr;
}

template <typename T>
void matrix<T>::print_matrix()
{
    cout<<"State\t\t"<<"U\t\t"<<"D\t\t"<<"L\t\t"<<"R\n";
    std::ostringstream out; 
    for(int i=0;i<rows;i++)
    {
        cout<<"S"<<(i+1)<<"\t\t";
        for(int j=0;j<cols;j++)
        {
            if(matr[i][j] >1 || matr[i][j] == -99 || matr[i][j]==0)
                cout<<matr[i][j]<<"\t\t";
            else
                cout<<matr[i][j]<<"\t";
        }
        cout<<endl;
    }
}

template <typename T>
void matrix<T>::init_matrix()
{
    for(int i=0;i<rows;i++)
    {
        for(int j=0;j<cols;j++)
        {
            matr[i][j] = 0;
        }
     }
}

template <typename T>
int matrix<T>::maxaction(int state)
{
    int maxAction = LEFT;
    double maxQvalue = 0;
    for(int action = 0; action<ACTIONMAX;action++)
    {
        if(matr[state][action]>maxQvalue)
        {
            maxQvalue = matr[state][action];
            maxAction = action;
        }
    }
    return maxAction;
}

template <typename T>
double matrix<T>::maxi(int nextState)
{
    double maxi = 0;
    for(int action=0;action<ACTIONMAX;action++)
    {
        if(matr[nextState][action]>maxi)
            maxi = matr[nextState][action];
    }
    return maxi;
}

template <typename T>
void matrix<T>::init_row_random(int row, int *valid_actions)
{
    for(int j=0;j<ACTIONMAX;j++)
    {
        if(!valid_actions[j])
        {
            matr[row][j] = -99.0;
        } else {
            matr[row][j] = (get_random_zo());
        }
    }
}

template <typename T>
double matrix<T>::get_random_zo(void)
{
    return ((double) rand() / (RAND_MAX));
}

template <typename T>
void matrix<T>::init_row(int row, int *valid_actions)
{
    for(int j=0;j<ACTIONMAX;j++)
    {
        if(!valid_actions[j])
        {
            matr[row][j] = -99.0;
        } else {
            matr[row][j] = 0.0;
        }
    }
}

int getNextState(int state, int action)
{
    int nextState;
    int row = state/3;
    int col = state%3;
    switch(action)
    {
        case LEFT:
           col-=1; 
            break;
        case DOWN:
            row+=1;
            break;
        case RIGHT:
            col++;
            break;
        case UP:
            row--;
            break;
    }
    nextState = row*3+col;
    return nextState;
}

int *get_valid_actions(int state)
{
    /*
     *  L D R U
     *  1 1 1 1
     */
    int *valid_actions = new int[ACTIONMAX];
    int row = state/3;
    int col = state%3;
    for(int i = 0;i<ACTIONMAX; i++)
        valid_actions[i] = 0;
    if(row>RCMIN)
        valid_actions[UP] = 1;
    if(row<RCMAX)
        valid_actions[DOWN] = 1;
    if(col>RCMIN)
        valid_actions[LEFT] = 1;
    if(col<RCMAX)
        valid_actions[RIGHT] = 1;
    return valid_actions;
}

template <typename T, size_t N>
inline
size_t SizeOfArray( const T(&)[ N ] )
{
      return N;
}

int get_random(int min, int max)
{
    return min + (rand() % (int)(max - min + 1));
}

int stochastic_prob(int state, int intendAction, double prob)
{
    /*      0.7        0.3/n 0.3/n 0.3/n
     * ...............|.....|.....|.....|
     */
    int *valid_actions;
    int defence, NumOfValidActions = 0;

    valid_actions = get_valid_actions(state);
    valid_actions[intendAction] = 0;

    for(int i=0;i<ACTIONMAX;i++)
    {
        if(valid_actions[i])
            NumOfValidActions++;
    }

    if(prob<=0.7)
        return intendAction;
    prob-=0.7;
    
    for(int action=0;action<ACTIONMAX;action++)
    {
        if(valid_actions[action]) {
            defence = action;
            if(prob<=(0.3/NumOfValidActions))
                return action;
            else
                prob-=(0.3/NumOfValidActions);
        }
    }
    return defence;
}

int main()
{
    matrix<double> *QMatrix;
    matrix<double> *Expectr;
    matrix<int> *visits;
    int *valid_actions;
    int *Sval = new int[STATEMAX];
    int iter = 0;
    int nextState;
    int action, stochaction;
    int state;
    int nochange = 0;
    double update, random;
    float gamma = 0.9, alphan;
    float epsilon[] = {0.0, 0.2, 0.5, 1.0};
    
    srand (time(NULL));
    QMatrix  = new matrix<double>(STATEMAX, ACTIONMAX);
    Expectr  = new matrix<double>(STATEMAX, ACTIONMAX);
    visits  = new matrix<int>(STATEMAX, ACTIONMAX);
    visits->init_matrix();
    
    for(int i=0;i<4;i++)
    {
        /*
         * Initialize all the related values
         */
        Expectr->init_matrix();
        for(int st=0;st<STATEMAX;st++)
        {
            Sval[st] = 0;
            valid_actions = get_valid_actions(st);
            QMatrix->init_row_random(st, valid_actions);
            delete(valid_actions);
        }
        Sval[7] = 100;
        state = get_random(STATEMIN, STATEMAX-1);
        
        /*
         * Update QMatrix in each iteration
         */
        while(1)
        {
            state = get_random(STATEMIN, STATEMAX-1);
            if(nochange==150000 || iter == 100000)
                break;
            action = get_random(ACTIONMIN, ACTIONMAX-1);
            random = QMatrix->get_random_zo();

            visits->matr[state][action]= visits->matr[state][action]+1;
            if(state == 7)
            {
                state = get_random(STATEMIN, STATEMAX-1);
                continue;
            }
            
            /*
             * With a prob it will be go to different states from the current state
             */
            if(QMatrix->matr[state][action] !=-99)
            {
                stochaction = stochastic_prob(state, action, random);
                alphan = (1.0/(1+visits->matr[state][stochaction]));
                nextState = getNextState(state, stochaction);
                Expectr->matr[state][action] = Expectr->matr[state][action] + Sval[nextState];
                update = (1-alphan)*(QMatrix->matr[state][action]) + \
                         (alphan)*(Sval[nextState]+ gamma*(QMatrix->maxi(nextState)));
                
                if(QMatrix->matr[state][action] - update < 0.001)
                    nochange++;
                else
                    nochange=0;
                QMatrix->matr[state][action] = update;
            }
            iter++;
            //state = nextState;
        }
        
        for(int state = 0; state < STATEMAX; state++)
        {
            for(int action = 0; action < ACTIONMAX; action++)
            {
                if(visits->matr[state][action] > 0)
                    Expectr->matr[state][action] = Expectr->matr[state][action] / visits->matr[state][action];
            }
        }

        cout<<"For epsilon = "<<epsilon[i]<<endl;
        cout<<"Num of iterations"<<iter<<endl;
        QMatrix->print_matrix();
        Expectr->print_matrix();
        //visits->print_matrix();
        cout<<endl;
        iter = 0;
        nochange=0;
        break;
    }
    
    return 0;
}


