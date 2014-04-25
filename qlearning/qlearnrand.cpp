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
    double maxi(int);
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
    cout<<"State\t"<<"U\t"<<"D\t"<<"L\t"<<"R\n";
    for(int i=0;i<rows;i++)
    {
        cout<<"S"<<(i+1)<<"\t";
        for(int j=0;j<cols;j++)
        {
            cout<<matr[i][j]<<'\t';
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
void matrix<T>::init_row(int row, int *valid_actions)
{
    for(int j=0;j<ACTIONMAX;j++)
    {
        if(!valid_actions[j] || row ==7)
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

int get_random(int min, int max)
{
    return min + (rand() % (int)(max - min + 1));
}

int main()
{
    matrix<double> *QMatrix;
    int *valid_actions;
    int *Sval = new int[STATEMAX];
    int iter = 0;
    int nextState;
    int action;
    int state;
    int nochange = 0;
    double update, towrite;
    float gamma = 0.9;
    FILE *out;

    out = fopen("random", "w");
    srand (time(NULL));
    QMatrix  = new matrix<double>(STATEMAX, ACTIONMAX);

    /*
     * Initialize all the related values
     */
    for(int state=0;state<STATEMAX;state++)
    {
        Sval[state] = 0;
        valid_actions = get_valid_actions(state);
        QMatrix->init_row(state, valid_actions);
        cout<<"S"<<(state+1)<<"\t";
        for(int action=0;action<ACTIONMAX;action++)
           cout<<valid_actions[action]<<'\t';
        delete(valid_actions);
        cout<<'\n';
    }
    Sval[7] = 100;
    
    /*
     * Update QMatrix in each iteration
     */
    state = get_random(STATEMIN, STATEMAX-1);
    while(1)
    {
        action = get_random(ACTIONMIN, ACTIONMAX-1);
        if(state == 7)
        {
            state = get_random(STATEMIN, STATEMAX-1);
            continue;
        }
        if(QMatrix->matr[state][action] !=-99)
        {
            nextState = getNextState(state, action);
            update = Sval[nextState] + gamma*(QMatrix->maxi(nextState));
            if(QMatrix->matr[state][action] - update == 0)
                nochange++;
            else
                nochange=0;
            towrite = abs(QMatrix->matr[state][action] - update);
            fprintf(out, "%f\n", towrite);
            QMatrix->matr[state][action] = update;
        }
        if(nochange==50)
            break;
        iter++;
        state = nextState;
    }
    
    cout<<"Num of iterations"<<iter<<endl;
    QMatrix->print_matrix();
    return 0;
}


