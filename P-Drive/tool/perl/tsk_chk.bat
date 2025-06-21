perl p:\tool\perl\f_arg.pl *.cod > f_arg
perl p:\tool\perl\f_auto.pl *.cod > f_auto
perl p:\tool\perl\t_stk.pl *.cod > t_stk
rem 1997.09.24 by RN perl p:\tool\perl\f_tree.pl *.cod | sort> f_tree
perl p:\tool\perl\f_tree.pl *.cod > f_tree
perl p:\tool\perl\tsk_tre3.pl f_arg f_auto f_tree f_tree>tsk_tree
perl p:\tool\perl\tsk_stk.pl f_arg f_auto f_tree f_tree>tsk_stk
perl p:\tool\perl\tsk_stk2.pl f_arg f_auto f_tree f_tree | sort >stksize.h
perl p:\tool\perl\tsk_stk3.pl f_arg f_auto f_tree t_stk f_tree>stk_tbl
