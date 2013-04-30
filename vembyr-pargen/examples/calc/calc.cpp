


#include <list>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string.h>


namespace Parser{



struct Value{
    typedef std::list<Value>::const_iterator iterator;

    Value():
        which(1),
        value(0){
    }

    Value(const Value & him):
    which(him.which),
    value(0){
        if (him.isData()){
            value = him.value;
        }
        if (him.isList()){
            values = him.values;
        }
    }

    explicit Value(const void * value):
        which(0),
        value(value){
    }

    Value & operator=(const Value & him){
        which = him.which;
        if (him.isData()){
            value = him.value;
        }
        if (him.isList()){
            values = him.values;
        }
        return *this;
    }

    Value & operator=(const void * what){
        this->value = what;
        return *this;
    }

    void reset(){
        this->value = 0;
        this->values.clear();
        this->which = 1;
    }

    int which; // 0 is value, 1 is values

    inline bool isList() const {
        return which == 1;
    }

    inline bool isData() const {
        return which == 0;
    }

    inline const void * getValue() const {
        return value;
    }

    inline void setValue(const void * value){
        which = 0;
        this->value = value;
    }

    inline const std::list<Value> & getValues() const {
        return values;
    }

    /*
    inline void setValues(std::list<Value> values){
        which = 1;
        values = values;
    }
    */

    const void * value;
    std::list<Value> values;
};

class Result{
public:
    Result():
    position(-2){
    }

    Result(const int position):
    position(position){
    }

    Result(const Result & r):
    position(r.position),
    value(r.value){
    }

    Result & operator=(const Result & r){
        position = r.position;
        value = r.value;
        return *this;
    }

    void reset(){
        value.reset();
    }

    void setPosition(int position){
        this->position = position;
    }

    inline int getPosition() const {
        return position;
    }

    inline bool error(){
        return position == -1;
    }

    inline bool calculated(){
        return position != -2;
    }

    inline void nextPosition(){
        position += 1;
    }

    void setError(){
        position = -1;
    }

    inline void setValue(const Value & value){
        this->value = value;
    }

    /*
    Value getLastValue() const {
        if (value.isList()){
            if (value.values.size() == 0){
                std::cout << "[peg] No last value to get!" << std::endl;
            }
            return value.values[value.values.size()-1];
        } else {
            return value;
        }
    }
    */

    inline int matches() const {
        if (value.isList()){
            return this->value.values.size();
        } else {
            return 1;
        }
    }

    inline const Value & getValues() const {
        return this->value;
    }

    void addResult(const Result & result){
        std::list<Value> & mine = this->value.values;
        mine.push_back(result.getValues());
        this->position = result.getPosition();
        this->value.which = 1;
    }

private:
    int position;
    Value value;
};



struct Chunk0{
Result chunk_start;
    Result chunk_expression;
    Result chunk_expression1_rest;
    Result chunk_expression2;
    Result chunk_expression2_rest;
};

struct Chunk1{
Result chunk_expression3;
};

struct Column{
Column():
    chunk0(0)
        ,chunk1(0){
}

Chunk0 * chunk0;
    Chunk1 * chunk1;

int hitCount(){
    return 0;
}

int maxHits(){
    return 6;
}

~Column(){
    delete chunk0;
        delete chunk1;
}
};


class ParseException: std::exception {
public:
    ParseException(const std::string & reason):
    std::exception(),
    line(-1), column(-1),
    message(reason){
    }

    ParseException(const std::string & reason, int line, int column):
    std::exception(),
    line(line), column(column),
    message(reason){
    }

    std::string getReason() const;
    int getLine() const;
    int getColumn() const;

    virtual ~ParseException() throw(){
    }

protected:
    int line, column;
    std::string message;
};

class Stream{
public:
    struct LineInfo{
        LineInfo(int line, int column):
        line(line),
        column(column){
        }

        LineInfo(const LineInfo & copy):
        line(copy.line),
        column(copy.column){
        }

        LineInfo():
        line(-1),
        column(-1){
        }

        int line;
        int column;
    };

public:
    /* read from a file */
    Stream(const std::string & filename):
    temp(0),
    buffer(0),
    farthest(0),
    last_line_info(-1){
        std::ifstream stream;
        /* ios::binary is needed on windows */
        stream.open(filename.c_str(), std::ios::in | std::ios::binary);
        if (stream.fail()){
            std::ostringstream out;
            out << __FILE__  << " cannot open '" << filename << "'";
            throw ParseException(out.str());
        }
        stream.seekg(0, std::ios_base::end);
        max = stream.tellg();
        stream.seekg(0, std::ios_base::beg);
        temp = new char[max];
        stream.read(temp, max);
        buffer = temp;
        stream.close();

        line_info[-1] = LineInfo(1, 1);

        createMemo();
    }

    /* for null-terminated strings */
    Stream(const char * in):
    temp(0),
    buffer(in),
    farthest(0),
    last_line_info(-1){
        max = strlen(buffer);
        line_info[-1] = LineInfo(1, 1);
        createMemo();
    }

    /* user-defined length */
    Stream(const char * in, int length):
    temp(0),
    buffer(in),
    farthest(0),
    last_line_info(-1){
        max = length;
        line_info[-1] = LineInfo(1, 1);
        createMemo();
    }

    void createMemo(){
        memo_size = 1024 * 2;
        memo = new Column*[memo_size];
        /* dont create column objects before they are needed because transient
         * productions will never call for them so we can save some space by
         * not allocating columns at all.
         */
        memset(memo, 0, sizeof(Column*) * memo_size);
        /*
        for (int i = 0; i < memo_size; i++){
            memo[i] = new Column();
        }
        */
    }

    int length(){
        return max;
    }

    /* prints statistics about how often rules were fired and how
     * likely rules are to succeed
     */
    void printStats(){
        double min = 1;
        double max = 0;
        double average = 0;
        int count = 0;
        for (int i = 0; i < length(); i++){
            Column & c = getColumn(i);
            double rate = (double) c.hitCount() / (double) c.maxHits();
            if (rate != 0 && rate < min){
                min = rate;
            }
            if (rate > max){
                max = rate;
            }
            if (rate != 0){
                average += rate;
                count += 1;
            }
        }
        std::cout << "Min " << (100 * min) << " Max " << (100 * max) << " Average " << (100 * average / count) << " Count " << count << " Length " << length() << " Rule rate " << (100.0 * (double)count / (double) length()) << std::endl;
    }

    char get(const int position){
        if (position >= max || position < 0){
            return '\0';
        }

        // std::cout << "Read char '" << buffer[position] << "'" << std::endl;

        return buffer[position];
        /*
        char z;
        stream.seekg(position, std::ios_base::beg);
        stream >> z;
        return z;
        */
    }

    bool find(const char * str, const int position){
        if (position >= max || position < 0){
            return false;
        }
        return strncmp(&buffer[position], str, max - position) == 0;
    }

    void growMemo(){
        int newSize = memo_size * 2;
        Column ** newMemo = new Column*[newSize];
        memcpy(newMemo, memo, sizeof(Column*) * memo_size);
        memset(&newMemo[memo_size], 0, sizeof(Column*) * (newSize - memo_size));
        /*
        for (int i = memo_size; i < newSize; i++){
            newMemo[i] = new Column();
        }
        */
        delete[] memo;
        memo = newMemo;
        memo_size = newSize;
    }

    /* I'm sure this can be optimized. It only takes into account
     * the last position used to get line information rather than
     * finding a position closest to the one asked for.
     * So if the last position is 20 and the current position being requested
     * is 15 then this function will compute the information starting from 0.
     * If the information for 10 was computed then that should be used instead.
     * Maybe something like, sort the positions, find closest match lower
     * than the position and start from there.
     */
    LineInfo makeLineInfo(int last_line_position, int position){
        int line = line_info[last_line_position].line;
        int column = line_info[last_line_position].column;
        for (int i = last_line_position + 1; i < position; i++){
            if (buffer[i] == '\n'){
                line += 1;
                column = 1;
            } else {
                column += 1;
            }
        }
        return LineInfo(line, column);
    }

    void updateLineInfo(int position){
        if (line_info.find(position) == line_info.end()){
            if (position > last_line_info){
                line_info[position] = makeLineInfo(last_line_info, position);
            } else {
                line_info[position] = makeLineInfo(0, position);
            }
            last_line_info = position;
        }
    }

    const LineInfo & getLineInfo(int position){
        updateLineInfo(position);
        return line_info[position];
    }

    /* throws a ParseException */
    void reportError(const std::string & parsingContext){
        std::ostringstream out;
        int line = 1;
        int column = 1;
        for (int i = 0; i < farthest; i++){
            if (buffer[i] == '\n'){
                line += 1;
                column = 1;
            } else {
                column += 1;
            }
        }
        int context = 15;
        int left = farthest - context;
        int right = farthest + context;
        if (left < 0){
            left = 0;
        }
        if (right >= max){
            right = max;
        }
        out << "Error while parsing " << parsingContext << ". Read up till line " << line << " column " << column << std::endl;
        std::ostringstream show;
        for (int i = left; i < right; i++){
            char c = buffer[i];
            switch (buffer[i]){
                case '\n' : {
                    show << '\\';
                    show << 'n';
                    break;
                }
                case '\r' : {
                    show << '\\';
                    show << 'r';
                    break;
                }
                case '\t' : {
                    show << '\\';
                    show << 't';
                    break;
                }
                default : show << c; break;
            }
        }
        out << "'" << show.str() << "'" << std::endl;
        for (int i = 0; i < farthest - left; i++){
            out << " ";
        }
        out << "^" << std::endl;
        out << "Last successful rule trace" << std::endl;
        out << makeBacktrace() << std::endl;
        throw ParseException(out.str(), line, column);
    }

    std::string makeBacktrace(){
        std::ostringstream out;

        bool first = true;
        for (std::vector<std::string>::iterator it = last_trace.begin(); it != last_trace.end(); it++){
            if (!first){
                out << " -> ";
            } else {
                first = false;
            }
            out << *it;
        }

        return out.str();
    }

    inline Column & getColumn(const int position){
        while (position >= memo_size){
            growMemo();
        }
        /* create columns lazily because not every position will have a column. */
        if (memo[position] == NULL){
            memo[position] = new Column();
        }
        return *(memo[position]);
    }

    void update(const int position){
        if (position > farthest){
            farthest = position;
            last_trace = rule_backtrace;
        }
    }

    void push_rule(const char * name){
        rule_backtrace.push_back(name);
    }

    void pop_rule(){
        rule_backtrace.pop_back();
    }

    ~Stream(){
        delete[] temp;
        for (int i = 0; i < memo_size; i++){
            delete memo[i];
        }
        delete[] memo;
    }

private:
    char * temp;
    const char * buffer;
    /* an array is faster and uses less memory than std::map */
    Column ** memo;
    int memo_size;
    int max;
    int farthest;
    std::vector<std::string> rule_backtrace;
    std::vector<std::string> last_trace;
    int last_line_info;
    std::map<int, LineInfo> line_info;
};

static int getCurrentLine(const Value & value){
    Stream::LineInfo * info = (Stream::LineInfo*) value.getValue();
    return info->line;
}

static int getCurrentColumn(const Value & value){
    Stream::LineInfo * info = (Stream::LineInfo*) value.getValue();
    return info->column;
}

class RuleTrace{
public:
    RuleTrace(Stream & stream, const char * name):
    stream(stream){
        stream.push_rule(name);
    }

    ~RuleTrace(){
        stream.pop_rule();
    }

    Stream & stream;
};

static inline bool compareChar(const char a, const char b){
    return a == b;
}

static inline char lower(const char x){
    if (x >= 'A' && x <= 'Z'){
        return x - 'A' + 'a';
    }
    return x;
}

static inline bool compareCharCase(const char a, const char b){
    return lower(a) == lower(b);
}


std::string ParseException::getReason() const {
return message;
}

int ParseException::getLine() const {
return line;
}

int ParseException::getColumn() const {
return column;
}

Result errorResult(-1);

Result rule_start(Stream &, const int);
Result rule_expression(Stream &, const int);
Result rule_expression1_rest(Stream &, const int, Value a);
Result rule_expression2(Stream &, const int);
Result rule_expression2_rest(Stream &, const int, Value a);
Result rule_expression3(Stream &, const int);


static Value add(const Value & a, const Value & b){
	return Value((void*)((int) a.getValue() + (int) b.getValue()));
}
static Value sub(const Value & a, const Value & b){
	return Value((void*)((int) a.getValue() - (int) b.getValue()));
}
static Value multiply(const Value & a, const Value & b){
	return Value((void*)((int) a.getValue() * (int) b.getValue()));
}
static Value divide(const Value & a, const Value & b){
	return Value((void*)((int) a.getValue() / (int) b.getValue()));
}



Result rule_start(Stream & stream, const int position){
    
    Column & column_peg_1 = stream.getColumn(position);
    if (column_peg_1.chunk0 != 0 && column_peg_1.chunk0->chunk_start.calculated()){
        return column_peg_1.chunk0->chunk_start;
    }
    
    RuleTrace trace_peg_14(stream, "start");
    int myposition = position;
    
    
    
    Result result_peg_2(myposition);
        
        {
        
            result_peg_2 = rule_expression(stream, result_peg_2.getPosition());
                if (result_peg_2.error()){
                    goto out_peg_4;
                }
            
            Result result_peg_3 = result_peg_2;
            
            {
                    int position_peg_8 = result_peg_2.getPosition();
                    
                    result_peg_2.reset();
                    do{
                        Result result_peg_12(result_peg_2.getPosition());
                        result_peg_12.setValue(Value((void*) "\\n"));
                        for (int i = 0; i < 3; i++){
                            if (compareChar("\\n"[i], stream.get(result_peg_12.getPosition()))){
                                result_peg_12.nextPosition();
                            } else {
                                goto loop_peg_11;
                            }
                        }
                        result_peg_2.addResult(result_peg_12);
                    } while (true);
                    loop_peg_11:
                    ;
                            
                }
                goto success_peg_6;
                goto out_peg_4;
                success_peg_6:
                ;
            
            
            
            if ('\0' == stream.get(result_peg_2.getPosition())){
                    result_peg_2.nextPosition();
                    result_peg_2.setValue(Value((void *) '\0'));
                } else {
                    goto out_peg_4;
                }
            
            
            
            {
                    Value value((void*) 0);
                    value = result_peg_3.getValues();
                    result_peg_2.setValue(value);
                }
            
            
        }
        
        if (column_peg_1.chunk0 == 0){
            column_peg_1.chunk0 = new Chunk0();
        }
        column_peg_1.chunk0->chunk_start = result_peg_2;
        stream.update(result_peg_2.getPosition());
        
        
        return result_peg_2;
        out_peg_4:
    
        if (column_peg_1.chunk0 == 0){
            column_peg_1.chunk0 = new Chunk0();
        }
        column_peg_1.chunk0->chunk_start = errorResult;
        stream.update(errorResult.getPosition());
        
    
    return errorResult;
}
        

Result rule_expression(Stream & stream, const int position){
    
    Column & column_peg_1 = stream.getColumn(position);
    if (column_peg_1.chunk0 != 0 && column_peg_1.chunk0->chunk_expression.calculated()){
        return column_peg_1.chunk0->chunk_expression;
    }
    
    RuleTrace trace_peg_5(stream, "expression");
    int myposition = position;
    
    
    
    Result result_peg_2(myposition);
        
        {
        
            result_peg_2 = rule_expression2(stream, result_peg_2.getPosition());
                if (result_peg_2.error()){
                    goto out_peg_4;
                }
            
            Result result_peg_3 = result_peg_2;
            
            result_peg_2 = rule_expression1_rest(stream, result_peg_2.getPosition(), result_peg_3.getValues());
                if (result_peg_2.error()){
                    goto out_peg_4;
                }
            
            
        }
        
        if (column_peg_1.chunk0 == 0){
            column_peg_1.chunk0 = new Chunk0();
        }
        column_peg_1.chunk0->chunk_expression = result_peg_2;
        stream.update(result_peg_2.getPosition());
        
        
        return result_peg_2;
        out_peg_4:
    
        if (column_peg_1.chunk0 == 0){
            column_peg_1.chunk0 = new Chunk0();
        }
        column_peg_1.chunk0->chunk_expression = errorResult;
        stream.update(errorResult.getPosition());
        
    
    return errorResult;
}
        

Result rule_expression1_rest(Stream & stream, const int position, Value a){
    
    RuleTrace trace_peg_15(stream, "expression1_rest");
    int myposition = position;
    
    tail_peg_3:
    Value e;
    Result result_peg_2(myposition);
        {
        
            result_peg_2.setValue(Value((void*) "+"));
                for (int i = 0; i < 1; i++){
                    if (compareChar("+"[i], stream.get(result_peg_2.getPosition()))){
                        result_peg_2.nextPosition();
                    } else {
                        goto out_peg_5;
                    }
                }
            
            
            
            result_peg_2 = rule_expression2(stream, result_peg_2.getPosition());
                if (result_peg_2.error()){
                    goto out_peg_5;
                }
            
            Result result_peg_6 = result_peg_2;
            
            {
                    Value value((void*) 0);
                    value = add(a,result_peg_6.getValues());
                    result_peg_2.setValue(value);
                }
                e = result_peg_2.getValues();
            
            
            
            a = e;
            
            
        }
        myposition = result_peg_2.getPosition();
        goto tail_peg_3;
        out_peg_5:
        Result result_peg_8(myposition);
        {
        
            result_peg_8.setValue(Value((void*) "-"));
                for (int i = 0; i < 1; i++){
                    if (compareChar("-"[i], stream.get(result_peg_8.getPosition()))){
                        result_peg_8.nextPosition();
                    } else {
                        goto out_peg_10;
                    }
                }
            
            
            
            result_peg_8 = rule_expression2(stream, result_peg_8.getPosition());
                if (result_peg_8.error()){
                    goto out_peg_10;
                }
            
            Result result_peg_11 = result_peg_8;
            
            {
                    Value value((void*) 0);
                    value = sub(a,result_peg_11.getValues());
                    result_peg_8.setValue(value);
                }
                e = result_peg_8.getValues();
            
            
            
            a = e;
            
            
        }
        myposition = result_peg_8.getPosition();
        goto tail_peg_3;
        out_peg_10:
        Result result_peg_13(myposition);
        
        {
        
            
            
            
            
            {
                    Value value((void*) 0);
                    value = a;
                    result_peg_13.setValue(value);
                }
            
            
        }
        stream.update(result_peg_13.getPosition());
        
        return result_peg_13;
    stream.update(errorResult.getPosition());
    
    return errorResult;
}
        

Result rule_expression2(Stream & stream, const int position){
    
    Column & column_peg_1 = stream.getColumn(position);
    if (column_peg_1.chunk0 != 0 && column_peg_1.chunk0->chunk_expression2.calculated()){
        return column_peg_1.chunk0->chunk_expression2;
    }
    
    RuleTrace trace_peg_5(stream, "expression2");
    int myposition = position;
    
    
    
    Result result_peg_2(myposition);
        
        {
        
            result_peg_2 = rule_expression3(stream, result_peg_2.getPosition());
                if (result_peg_2.error()){
                    goto out_peg_4;
                }
            
            Result result_peg_3 = result_peg_2;
            
            result_peg_2 = rule_expression2_rest(stream, result_peg_2.getPosition(), result_peg_3.getValues());
                if (result_peg_2.error()){
                    goto out_peg_4;
                }
            
            
        }
        
        if (column_peg_1.chunk0 == 0){
            column_peg_1.chunk0 = new Chunk0();
        }
        column_peg_1.chunk0->chunk_expression2 = result_peg_2;
        stream.update(result_peg_2.getPosition());
        
        
        return result_peg_2;
        out_peg_4:
    
        if (column_peg_1.chunk0 == 0){
            column_peg_1.chunk0 = new Chunk0();
        }
        column_peg_1.chunk0->chunk_expression2 = errorResult;
        stream.update(errorResult.getPosition());
        
    
    return errorResult;
}
        

Result rule_expression2_rest(Stream & stream, const int position, Value a){
    
    RuleTrace trace_peg_15(stream, "expression2_rest");
    int myposition = position;
    
    tail_peg_3:
    Value e;
    Result result_peg_2(myposition);
        {
        
            result_peg_2.setValue(Value((void*) "*"));
                for (int i = 0; i < 1; i++){
                    if (compareChar("*"[i], stream.get(result_peg_2.getPosition()))){
                        result_peg_2.nextPosition();
                    } else {
                        goto out_peg_5;
                    }
                }
            
            
            
            result_peg_2 = rule_expression3(stream, result_peg_2.getPosition());
                if (result_peg_2.error()){
                    goto out_peg_5;
                }
            
            Result result_peg_6 = result_peg_2;
            
            {
                    Value value((void*) 0);
                    value = multiply(a,result_peg_6.getValues());
                    result_peg_2.setValue(value);
                }
                e = result_peg_2.getValues();
            
            
            
            a = e;
            
            
        }
        myposition = result_peg_2.getPosition();
        goto tail_peg_3;
        out_peg_5:
        Result result_peg_8(myposition);
        {
        
            result_peg_8.setValue(Value((void*) "/"));
                for (int i = 0; i < 1; i++){
                    if (compareChar("/"[i], stream.get(result_peg_8.getPosition()))){
                        result_peg_8.nextPosition();
                    } else {
                        goto out_peg_10;
                    }
                }
            
            
            
            result_peg_8 = rule_expression3(stream, result_peg_8.getPosition());
                if (result_peg_8.error()){
                    goto out_peg_10;
                }
            
            Result result_peg_11 = result_peg_8;
            
            {
                    Value value((void*) 0);
                    value = divide(a,result_peg_11.getValues());
                    result_peg_8.setValue(value);
                }
                e = result_peg_8.getValues();
            
            
            
            a = e;
            
            
        }
        myposition = result_peg_8.getPosition();
        goto tail_peg_3;
        out_peg_10:
        Result result_peg_13(myposition);
        
        {
        
            
            
            
            
            {
                    Value value((void*) 0);
                    value = a;
                    result_peg_13.setValue(value);
                }
            
            
        }
        stream.update(result_peg_13.getPosition());
        
        return result_peg_13;
    stream.update(errorResult.getPosition());
    
    return errorResult;
}
        

Result rule_expression3(Stream & stream, const int position){
    
    Column & column_peg_1 = stream.getColumn(position);
    if (column_peg_1.chunk1 != 0 && column_peg_1.chunk1->chunk_expression3.calculated()){
        return column_peg_1.chunk1->chunk_expression3;
    }
    
    RuleTrace trace_peg_31(stream, "expression3");
    int myposition = position;
    
    
    
    Result result_peg_2(myposition);
        
        {
            int position_peg_5 = result_peg_2.getPosition();
            {
            
                result_peg_2.reset();
                    do{
                        Result result_peg_18(result_peg_2.getPosition());
                        {
                            int position_peg_21 = result_peg_18.getPosition();
                            
                            char letter_peg_24 = stream.get(result_peg_18.getPosition());
                            if (letter_peg_24 != '\0' && strchr("0123456789", letter_peg_24) != NULL){
                                result_peg_18.nextPosition();
                                result_peg_18.setValue(Value((void*) (long) letter_peg_24));
                            } else {
                                result_peg_18.setPosition(position_peg_21);
                                goto out_peg_23;
                            }
                            
                        }
                        goto success_peg_19;
                        out_peg_23:
                        goto loop_peg_17;
                        success_peg_19:
                        ;
                        result_peg_2.addResult(result_peg_18);
                    } while (true);
                    loop_peg_17:
                    if (result_peg_2.matches() == 0){
                        result_peg_2.setPosition(position_peg_5);
                        goto out_peg_15;
                    }
                
                Result result_peg_16 = result_peg_2;
                
                {
                        Value value((void*) 0);
                        int total = 0;
                        	for (Value::iterator it = result_peg_16.getValues().getValues().begin(); it !=	result_peg_16.getValues().getValues().end(); it++){
                        		const Value & v = *it;
                        		char letter = (char) (int) v.getValue();
                        		total = (total * 10) + letter - '0';
                        	}
                        	value = (void*) total;
                        result_peg_2.setValue(value);
                    }
                
                
            }
        }
        goto success_peg_3;
        out_peg_15:
        goto out_peg_25;
        success_peg_3:
        ;
        
        if (column_peg_1.chunk1 == 0){
            column_peg_1.chunk1 = new Chunk1();
        }
        column_peg_1.chunk1->chunk_expression3 = result_peg_2;
        stream.update(result_peg_2.getPosition());
        
        
        return result_peg_2;
        out_peg_25:
        Result result_peg_26(myposition);
        
        {
        
            result_peg_26.setValue(Value((void*) "("));
                for (int i = 0; i < 1; i++){
                    if (compareChar("("[i], stream.get(result_peg_26.getPosition()))){
                        result_peg_26.nextPosition();
                    } else {
                        goto out_peg_28;
                    }
                }
            
            
            
            result_peg_26 = rule_expression(stream, result_peg_26.getPosition());
                if (result_peg_26.error()){
                    goto out_peg_28;
                }
            
            Result result_peg_29 = result_peg_26;
            
            result_peg_26.setValue(Value((void*) ")"));
                for (int i = 0; i < 1; i++){
                    if (compareChar(")"[i], stream.get(result_peg_26.getPosition()))){
                        result_peg_26.nextPosition();
                    } else {
                        goto out_peg_28;
                    }
                }
            
            
            
            {
                    Value value((void*) 0);
                    value = result_peg_29.getValues();
                    result_peg_26.setValue(value);
                }
            
            
        }
        
        if (column_peg_1.chunk1 == 0){
            column_peg_1.chunk1 = new Chunk1();
        }
        column_peg_1.chunk1->chunk_expression3 = result_peg_26;
        stream.update(result_peg_26.getPosition());
        
        
        return result_peg_26;
        out_peg_28:
    
        if (column_peg_1.chunk1 == 0){
            column_peg_1.chunk1 = new Chunk1();
        }
        column_peg_1.chunk1->chunk_expression3 = errorResult;
        stream.update(errorResult.getPosition());
        
    
    return errorResult;
}
        

static const void * doParse(Stream & stream, bool stats, const std::string & context){
errorResult.setError();
Result done = rule_start(stream, 0);
if (done.error()){
    stream.reportError(context);
}
if (stats){
    stream.printStats();
}
return done.getValues().getValue();
}

const void * parse(const std::string & filename, bool stats = false){
Stream stream(filename);
return doParse(stream, stats, filename);
}

const void * parse(const char * in, bool stats = false){
Stream stream(in);
return doParse(stream, stats, "memory");
}

const void * parse(const char * in, int length, bool stats = false){
Stream stream(in, length);
return doParse(stream, stats, "memory");
}



} /* Parser */

    
