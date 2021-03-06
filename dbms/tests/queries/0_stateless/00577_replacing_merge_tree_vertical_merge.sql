drop table if exists tab;
create table tab (date Date, version UInt64, val UInt64) engine = ReplacingMergeTree(version) partition by date order by date settings enable_vertical_merge_algorithm = 1, vertical_merge_algorithm_min_rows_to_activate = 1, vertical_merge_algorithm_min_columns_to_activate = 0;
insert into tab values ('2018-01-01', 2, 2), ('2018-01-01', 1, 1);
insert into tab values ('2018-01-01', 0, 0);
select * from tab order by version;
OPTIMIZE TABLE tab;
select * from tab;

