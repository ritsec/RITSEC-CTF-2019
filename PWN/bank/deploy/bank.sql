CREATE TABLE user (
    id integer primary key autoincrement not null,
    username text not null unique,
    hash blob not null,
    value signed integer not null default 100,
    verified integer not null default 0,
    proofOfWork char(40) not null
);

CREATE TABLE transfer (
    id integer primary key autoincrement not null,
    from_user integer not null,
    to_user integer not null,
    amount integer not null,
    validation_code char(10) not null,
    complete integer not null default 0,
    foreign key(from_user) references user(id),
    foreign key(to_user) references user(id)
);
