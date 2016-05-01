/*
	.set noat
	.set noreorder
	
	.data
	.align	2
	.type	old_rc, @object
	.size	old_rc, 4
old_rc:
	.space	4
        
        
        
     	.type	old_A, @object
        .size	old_A, 22
old_A:
    	.half	4096
   	.half	0
    	.half	0
   	.half	0
     	.half	0
     	.half	0
    	.half	0
     	.half	0
     	.half	0
     	.half	0
     	.half	0
*/

	.set noat
	.set noreorder

	.section .ydata,"aw",@nobits
	.align	2
	.origin	0

old_rc:	.half	0, 0	
old_A:	.half	4096, 0, 0, 0, 0, 0, 0, 0, 0, 0,0	// pre highpass filter (Q11) a140[3] = {1899, -3798, 1899}


