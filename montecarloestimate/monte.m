function function_name(range)
    pabtt = 0.0;
    pabtf = 0.0;
    pabff = 0.0;
    pabft = 0.0;
    pcth = 0.6;
    pbcth = 0.2;
    pacth = 0.9;
    pancth = 0.2;
    pbncth = 0.7;
    bt = 0;
    bf = 0;
    for i = 1:range
        pc = rand;
        c = 0;
        if (pc < pcth)
            c = 1;
            pac = rand;
            a = 0;
            if (pac < pacth)
                a = 1;
            end
            b = 0;
            pbc = rand;
            if (pbc < pbcth)
                b = 1;
            end
        else
            panc = rand;
            a = 0;
            if (panc < pancth)
                a = 1;
            end
            b= 0;
            pbnc = rand;
            if (pbnc < pbncth)
                b = 1;
            end
        end
        if(b)
            bt = bt +1;
            if(a)
                pabtt = pabtt+1;
            else
                pabft = pabft+1;
            end
        else
            bf = bf+1;
            if(a)
                pabtf = pabtf+1;
            else
                pabff = pabff+1;
            end
        end
    end
    s = sprintf('Samples %d', range);
    disp(s);
    disp(pabff/bf);
    disp(pabft/bt);
    disp(pabtf/bf);
    disp(pabtt/bt);
end

function_name(10);
function_name(100);
function_name(1000);

