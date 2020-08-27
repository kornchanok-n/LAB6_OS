echo -e "\nData Transfer Benchmarking\n"

echo "100 bytes"
for i in {1..10};
do
echo "testdata100.txt" | ./client $1 $2 | tail -2 | tr -d "\n" | awk -F" " '/in/{print $4}' 
done
echo -e "\n==================================\n"
echo "1K bytes"
for i in {1..10};
do
echo "testdata1k.txt" | ./client $1 $2 | tail -2 | tr -d "\n" | awk -F" " '/in/{print $4}' 
done
echo -e "\n==================================\n"
echo "10K bytes"
for i in {1..10};
do
echo "testdata10k.txt" | ./client $1 $2 | tail -2 | tr -d "\n" | awk -F" " '/in/{print $4}' 
done
echo -e "\n==================================\n"
echo "1M bytes"
for i in {1..10};
do
echo "testdata1m.txt" | ./client $1 $2 | tail -2 | tr -d "\n" | awk -F" " '/in/{print $4}' 
done
echo -e "\n==================================\n"
echo "10M bytes"
for i in {1..10};
do
echo "testdata10m.txt" | ./client $1 $2 | tail -2 | tr -d "\n" | awk -F" " '/in/{print $4}' 
done
echo -e "\n==================================\n"
echo "100M bytes"
for i in {1..10};
do
echo "testdata100m.txt" | ./client $1 $2 | tail -2 | tr -d "\n" | awk -F" " '/in/{print $4}' 
done
echo -e "\n==================================\n"
