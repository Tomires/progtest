awk '{print $4}' data_temp > 1.col;
awk '{print $2}' data_temp.txt > 2.col;
awk '(NR-2) % 2 {print $0}' data_fan.txt | awk '(NR+1) % 2 {print $0}' | awk '{print $2}' > 3.col;
awk 'NR % 2 == 0 {print substr($8,0,length($8)-1)}' data_load.txt | tr -s ',' '.' > 4.col;
paste -d\  1.col 2.col 3.col 4.col > almost.tit;
awk 'NR % 2 == 0 {print $0}' almost.tit > data.tit;


rm 1.col;
rm 2.col;
rm 3.col;
rm 4.col;
rm almost.tit;
