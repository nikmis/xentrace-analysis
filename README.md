INTRODUCTION
================
Virtual machines are becoming an integral part of both industrial data center facilities and large academic experimentation environments. It’s important to measure the overheads of virtualization and identify performance bottlenecks in the virtual setups. This project aspires to provide tools to perform fine-grained analysis of resource utilization, virtualization overheads, and performance bottlenecks, in a virtualized software stack.

PROJECT OVERVIEW
================
The goal of this project is to not just build a specific set of performance analysis tools, but to also give the developer a generic lightweight framework to write any type of performance analyses he or she desires.

The implementation is divided into 2 main components: Reader and Analyses. 
	- The Reader component parses the data inside the logs and neatly constructs C style data structures that are eventually passed to the Analyses component. 
	- The Analyses consists of a group of handlers(tools) for different types of events. These handlers are the only things that need to be written by the developer to build custom analysis. Some of the analysis tools implemented for this project are discussed below.

TOOLS
================
Some of the analysis tools being implemented for this project are listed below.

* CPU Utilization: Apart from the Physical CPU core usage, this analysis also shows CPU usage by each VM and their respective Virtual CPU (VCPU) utilization. Some scenarios where such granularity is necessary are:
	- Check if hypervisor configuration adheres to Service Level Agreements (SLA) between hosting providers and clients.
	- Detecting unbalanced mapping between physical CPUs and VMs.
* Time in Hypervisor: During an I/O intensive operation, a VM may spend much of its time being idle while the hypervisor does the work on its behalf. Hence this may give a wrong picture to someone who just relies on CPU usage data, as it doesn’t accurately represent the time spent in hypervisor. It can also help identify bottlenecks in the execution. (Ex: high latency I/O)
* CPU Scheduling Latency: Due to overscheduling of VCPUs or physical CPU being busy, VM context switches can get delayed. This analysis measures how much time a VM had to wait to get scheduled since the request to context switch was sent out.
* Disk I/O: A block request from a VM navigates through many different queues like the VM elevator queue, Xen Shared ring buffers, queues inside Dom 0 etc., before the request actually reaches the HDD. The response also traverses the same path. Hence monitoring the state of these queues can help identify overheads of virtualization and identify bottlenecks in the disk I/O pipeline.

TOOLS
================
All tools are listed under 'examples' directory. Below are their descriptions.

	* blk_io_queues 	: Disk I/O
	* cpu_time_sharing 	: CPU Utilization
	* cpu_wait 		: CPU Scheduling Latency
	* xen_dom_time_sharing 	: Time in Hypervisor
	* xen_stats 		: Xen Event stats.

