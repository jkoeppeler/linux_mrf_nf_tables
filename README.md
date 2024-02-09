# Linux MRF nf_tables kernel

Implementation of Move-Recursively-Forward algorithm in nf_tables based on this [research paper](https://arxiv.org/pdf/2109.15090.pdf)

### Activate MRF-nf_tables
Use MRF-nf_tables by selecting it in menuconfig under kernel hacking -> Self Adjusting List Configuration

### Configration
MRF-nf_tables creates two folders in sysfs for configurations, namely /sys/kernel/mrf_nft_config and /sys/kerenel/mrf_nft_api.
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

In mrf_nft_api are two files: mrf_cpu. This can be used to configure the information that is retrieved by the `nft list ruleset` command.
By writing a cpu number to mrf_cpu (e.g. `echo 2 > mrf_cpu`) the nft list command will return the list and order of the rules of the cpu specified by `mrf_cpu`.

