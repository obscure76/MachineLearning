
        /*for(int state=0;state<STATEMAX;state++)
        {
            for(int action=0;action<ACTIONMAX;action++)
            {
                if(QMatrix->matr[state][action] !=-99)
                {
                    nextState = getNextState(state, action);
                    QTemp->matr[state][action] = Sval[nextState] + gamma*(QMatrix->maxi(nextState));
                }
            }

            
             //Randomly selecting one action for a state in a given iteration
             
             action = get_random(ACTIONMIN, ACTIONMAX-1);
             if(QMatrix->matr[state][action] !=-99)
             {
                 nextState = getNextState(state, action);
                 QTemp->matr[state][action] = Sval[nextState] + gamma*(QMatrix->maxi(nextState));
             } 
             for(int rest=0;rest<ACTIONMAX;rest++)
             {
                 if(rest!=action)
                     QTemp->matr[state][rest] = QMatrix->matr[state][rest];
             }
        }*/
        
        /*
        for(int state=0;state<STATEMAX;state++)
        {
            for(int action=0;action<ACTIONMAX;action++)
            {
                QMatrix->matr[state][action] = QTemp->matr[state][action];
            }
        }*/
