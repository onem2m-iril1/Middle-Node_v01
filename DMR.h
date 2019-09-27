static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
   int i;
   for(i = 0; i<argc; i++) {
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   printf("\n");
   return 0;
}

static int retrive_ae(void *NotUsed, int argc, char **argv, char **azColName) {
   int i;
   for(i = 0; i<argc; i++) {
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
      if (i == 0) aei = argv[i];
      if (i == 1) api = argv[i];
      if (i == 2) rn = argv[i];
      if (i == 3) rr = argv[i];
   }
   printf(" \n");
   return 0;
}

static int retrive_cont(void *NotUsed, int argc, char **argv, char **azColName) {
   int i;
   for(i = 0; i<argc; i++) {
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
      if (i == 0) rn = argv[i];
      if (i == 1) _to = argv[i];
      if (i == 2) rqi = argv[i];
   }
   printf(" \n");
   return 0;
}

static int retrive_sub(void *NotUsed, int argc, char **argv, char **azColName) {
   int i;
   for(i = 0; i<argc; i++) {
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
      if (i == 0) rn = argv[i];
      if (i == 1) rqi = argv[i];
      if (i == 2) nu = argv[i];
      if (i == 3) net = std::stoi(argv[i]);
      if (i == 4) nct = std::stoi(argv[i]);
      if (i == 5) _to = argv[i];
   }
   printf(" \n");
   return 0;
}

void CreateTables(sqlite3 *db)
{
	int rc;
    char *zErrMsg = 0;
    char *sql;
    
    
    //Create Registration Table
    
    sql = "CREATE TABLE Registration("  \
      "aei TEXT PRIMARY KEY     NOT NULL," \
      "api           TEXT    NOT NULL," \
      "rn            TEXT     NOT NULL," \
      "rr            BOOLEAN     NOT NULL," \
      "ct            TEXT     NOT NULL," \
      "lt        TEXT NOT NULL);";
      
    rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
   
    if( rc != SQLITE_OK ){
       fprintf(stderr, "SQL error: %s\n", zErrMsg);
       sqlite3_free(zErrMsg);
    } 
    else        fprintf(stdout, "Registration Table created successfully\n");


	//Create Container Table

	sql = "CREATE TABLE Container("  \
      "rn		TEXT	PRIMARY KEY	NOT NULL," \
      "_to	TEXT     NOT NULL," \
      "rqi        TEXT	NOT NULL,"\
      "ct            TEXT     NOT NULL," \
      "lt            TEXT     NOT NULL," \
       "FOREIGN KEY (_to) REFERENCES Registration(aei) ON DELETE CASCADE);";
      
    rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
   
    if( rc != SQLITE_OK ){
       fprintf(stderr, "SQL error: %s\n", zErrMsg);
       sqlite3_free(zErrMsg);
    } 
    else        fprintf(stdout, "Container Table created successfully\n");
    
    //Create Content Instance Table
    
    sql = "CREATE TABLE ContentInstance("  \
      "rqi	TEXT	PRIMARY KEY	NOT NULL," \
      "con            TEXT     NOT NULL," \
      "cnf TEXT     NOT NULL," \
      "_to        TEXT		NOT NULL,"\
      "ct            TEXT     NOT NULL," \
      "lt            TEXT     NOT NULL," \
       "FOREIGN KEY (_to) REFERENCES Container(rn) ON DELETE CASCADE);";
      
    rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
   
    if( rc != SQLITE_OK ){
       fprintf(stderr, "SQL error: %s\n", zErrMsg);
       sqlite3_free(zErrMsg);
    } 
    else        fprintf(stdout, "ContentInstance Table created successfully\n");
    
    //Create Subscription Table
    
    sql = "CREATE TABLE Subscription("  \
      "rn	TEXT	PRIMARY KEY	NOT NULL," \
      "rqi            TEXT     NOT NULL," \
      "nu TEXT     NOT NULL," \
      "net INT     NOT NULL," \
      "nct INT     NOT NULL," \
      "_to        TEXT		NOT NULL,"\
      "ct            TEXT     NOT NULL," \
      "lt            TEXT     NOT NULL," \
       "FOREIGN KEY (_to) REFERENCES Container(rn) ON DELETE CASCADE);";
      
    rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
   
    if( rc != SQLITE_OK ){
       fprintf(stderr, "SQL error: %s\n", zErrMsg);
       sqlite3_free(zErrMsg);
    } 
    else        fprintf(stdout, "Subscription Table created successfully\n");
	
	sql = "PRAGMA foreign_keys = ON;";
	rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
	
	if( rc != SQLITE_OK ){
	   fprintf(stderr, "SQL error: %s\n", zErrMsg);
	   sqlite3_free(zErrMsg);
	} 
	else        fprintf(stdout, "Successfully invoked Foreign Keys Pragma\n");       
	
    return;
}
