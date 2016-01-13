#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H


class file_manager
{
    public:
        file_manager();
        char* path;
        const char* workspace="workspace";
        const char* data= "data";
        const char* training_data= "training_data";
        const char* input = "input";
        const char* desired_output = "desired_output";
        const char* scripts= "scripts";

    protected:
    private:
};

#endif // FILE_MANAGER_H
