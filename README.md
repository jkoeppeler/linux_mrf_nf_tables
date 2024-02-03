# Linux MRF nf_tables kernel

Implementation of Move-Recursively-Forward algorithm in nf_tables based on this [research paper](https://arxiv.org/pdf/2109.15090.pdf)

### Activate MRF-nf_tables
Use MRF-nf_tables by selecting it in menuconfig under kernel hacking -> Self Adjusting List Configuration

### Configration
MRF-nf_tables creates two folders in sysfs for configurations, namely /sys/kernel/mrf_nft_config and /sys/kerenel/mrf_nft_api .
In mrf_nft_config is a file mrf_enable. This can be used to enable the algorithm. It is disabled by default.
Enable
``` bash 
echo 1 > mrf_enable
```
Disable
``` bash
echo 0 > mrf_enable
```
Check the current status
``` bash
cat mrf_enable
```

In mrf_nft_api are two files: mrf_cpu and mrf_hook. These can be used to configure the information that is retrieved by the `nft list ruleset` command.
By writing a cpu number to mrf_cpu (e.g. `echo 2 > mrf_cpu`) the nft list command will return the list and order of the rules of that previously specified cpu.


**The mrf_hook file can be ignored for now** 
The mrf_hook file can be used to specify the NF_TABLES hook where the rules are stored.
- 0 = prerouting
- 1 = input
- 2 = forward
- 3 = output
- 4 = postrouting

Normally, to execute the experiments all rules should be in the prerouting hook.
