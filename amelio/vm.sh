VBoxManage setproperty machinefolder "/virtualbox/tmp"
cd VM1 
vagrant $* &
cd ../VM3 
vagrant $* &
cd ../VM1-6 
vagrant $* & 
cd ../VM2-6  
vagrant $* & 
cd ../VM3-6  
vagrant $* & 
cd ../

