x = 0.01:0.01:1;
y = 0.01:0.01:1;
[a,b] = meshgrid(x,y);
ey =  zeros(size(a));
maxe = 0;
maxx = 0;
maxy = 0;
for ia = 1:size(a)
    for ib = 1:size(b)
        pa = a(ia);
        pb = b(ib);
        if(pa+pb < 1.0)
            ey(ia,ib) = pa*log(pa)-pb*log(pb)-(1-pa-pb)*log(1.0-pa-pb);
            if(ey(ia,ib) > maxe)
                maxe = ey(ia,ib);
                maxx = pa;
                maxy = pb;
            end
        end
    end
end
maxe
maxx
maxy
