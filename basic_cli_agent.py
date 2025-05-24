import os
from langchain_google_genai import ChatGoogleGenerativeAI
from langchain_core.messages import HumanMessage, AIMessage
from langchain_core.prompts import ChatPromptTemplate, MessagesPlaceholder
from langchain.agents import create_tool_calling_agent, AgentExecutor
from langchain_core.tools import tool # Import the tool decorator
from datetime import datetime
import subprocess
import sys
from langchain_community.tools.tavily_search import TavilySearchResults

def now():
    return datetime.now().strftime('%Y-%m-%d %H:%M:%S.%f')


plogf = open("process_log.txt", "a")
hlogf = open("history_log.txt", "a")

plogf.write("Process Log started at: " + now() + "\n")
hlogf.write("History Log started at: " + now() + "\n")



# --- Agent Setup ---
plogf.write(now() +"\t Step 1: Verifying Gemini \\ GOOGLE API Key...\n")
# Ensure GOOGLE_API_KEY environment variable is set
if not os.getenv("GOOGLE_API_KEY"):
    print("Error: GOOGLE_API_KEY environment variable not set. Please set it.")
    exit()
plogf.write(now() +"\t Verified Gemini \\ GOOGLE API Key...\n")
#------------------------------------------------------------------------------------------------------
plogf.write(now() +"\t Step 2: Loading Gemini Model...\n")
# Load the Gemini model (using a model that supports tool calling, like gemini-2.0-flash or gemini-pro)
llm = ChatGoogleGenerativeAI(model="gemini-2.0-flash") # Using the model you tested in Step 2.1
plogf.write(now() +"\t Loaded Gemini Model...\n")
#------------------------------------------------------------------------------------------------------
plogf.write(now() +"\t Step 3: Creating Chat Prompt Template...\n")
# Create a chat prompt template for the agent
# This prompt must include the agent_scratchpad placeholder for tool use
prompt = ChatPromptTemplate.from_messages([
    ("system", "You are a helpful AI agent interacting via a command line. You have access to tools to help you answer questions or perform tasks."),
    MessagesPlaceholder(variable_name="chat_history"), # Placeholder for conversation history
    ("human", "{input}"), # Placeholder for the current user input
    MessagesPlaceholder(variable_name="agent_scratchpad"), # CRUCIAL: Placeholder for agent's internal thoughts and tool interactions
])
plogf.write(now() +"\t Created Chat Prompt Template...\n")
#------------------------------------------------------------------------------------------------------
plogf.write(now() +"\t Step 4: Defining Tools...\n")

# --- Define Tools ---
# Use the @tool decorator to turn a Python function into a LangChain tool
@tool
def read_files_folders(path: str) -> str:
    """Reads the content of a file at the given file_path.
    Returns the content of the file or an error message if the file cannot be read."""
    plogf.write(now() +"\t 46: Step 5: Defining Tool to  read files andfolders...\n")
    if isinstance(path, str):
        
        if not os.path.exists(path):
            plogf.write(now() +"\t 50: File not found at: " + path + "\n")
            return f"Error: File not found at {path}"
        plogf.write(now() +"\t 52: file/folder path :"+path+"is valid.\n")
        if os.path.isfile(path):
            try:
                # Read the file content
                plogf.write(now() +"\t 56: "+path + " is a file\n")
                with open(path, 'r', encoding='utf-8') as f:
                    content = f.read()
                plogf.write(now() +"\t 59: Read file content successfully\n")
                return f"Content of '{path}':\n---\n{content}\n---"
            except Exception as e:
                plogf.write(now() +"\t 62: Error reading file "+ str(e)+"\n")
                return f"Error reading file '{path}': {e}"
        elif os.path.isdir(path):
            # If it's a directory, list the files in it
            plogf.write(now() +"\t 66: "+path + " is a folder\n")
            try:
                files = os.listdir(path)
                for f in files:
                    plogf.write(now() +"\t 73: file :"+f+"\n")
                with open(f, 'r', encoding='utf-8') as f:
                    content = f.read()
                    contents = f.name + "\n" + content + "\n"
                plogf.write(now() +"\t 73: Read folder content successfully\n")
                return f"Content in files in '{path}':\n---\n{contents}\n---\n"
            except Exception as e:
                plogf.write(now() +"\t 75: Error listing files in directory "+ str(e)+"\n")
                return f"Error listing files in directory '{path}': {e}"
        else:
            plogf.write(now() +"\t 78: "+path + " is neither a file nor a folder currently.\n")
            return f"Error: '{path}' is neither a file nor a directory currently."
    # elif isinstance(paths, list):
    #     try:
    #         for path in paths:
    #             print("path",path)
    #             plogf.write("path :"+path+"\n")
    #     except Exception as e:
    #         plogf.write("Error listing files "+ str(e)+"\n")
    #         return f"Error listing files : {e}"

@tool
def edit_files_folders(path: str, content: str) -> str:
    """Writes the content to a file at the given file_path.
    Returns a success message or an error message if the file cannot be written."""
    plogf.write(now() +"\t Step 5: Defining Tool to write files and folders...\n")
    if isinstance(path, str):
        if not os.path.exists(path):
            plogf.write(now() +"\t File not found at: " + path + "\n")
            return f"Error: File not found at {path}"
        plogf.write(now() +"\t file/folder path :"+path+"is valid.\n")
        try:
            # Read the file content
            plogf.write(now() +"\t file is a file\n")
            with open(path, 'w', encoding='utf-8') as f:
                f.write(content)
            plogf.write(now() +"\t Read file content successfully\n")
            return f"Content of '{path}':\n---\n{content}\n---"
        except Exception as e:
            plogf.write(now() +"\t Error reading file "+ str(e)+"\n")
            return f"Error reading file '{path}': {e}"

@tool
def create_files_folders(path: str, content: str = None) -> str:
    """Creates a new file or a new directory (folder) at the specified path.
    If creating a file, provide content to write into it.
    If creating a folder, do not provide content.
    If the file or folder already exists, it will not raise an error (files will be overwritten if content is provided).
    Returns a success message or an error."""
    try:
        if content is None:
            # Assume the user wants to create a folder
            if os.path.exists(path):
                if os.path.isdir(path):
                    return f"Folder already exists at: {path}"
                else:
                    return f"Error: A file already exists at {path}. Cannot create folder."

            os.makedirs(path, exist_ok=True) # exist_ok=True prevents error if folder exists
            return f"Folder created successfully at: {path}"
        else:
            # Assume the user wants to create/write to a file
            folder_name = os.path.dirname(path)
            if folder_name: # If the path includes a directory, ensure it exists
                os.makedirs(folder_name, exist_ok=True)

            with open(path, 'w', encoding='utf-8') as f:
                f.write(content)
            return f"File created/written successfully at: {path}"

    except Exception as e:
        return f"Error creating file/folder or writing to {path}: {e}"

@tool
def run_terminal_command(command: str) -> str:
    """Executes a terminal command on the operating system.
    Returns the command's output or an error message.
    **USE THIS TOOL WITH EXTREME CAUTION.**
    Executing arbitrary commands can be dangerous. Only allow trusted inputs.
    """
    print(f"\nExecuting command: `{command}`\n") # Print the command being executed

    # Determine the appropriate shell based on the OS
    # On Windows, use 'cmd.exe' with '/c' or 'powershell.exe'
    # Using shell=True can be simpler but is generally less secure if inputs aren't trusted.
    # For this example, we'll use shell=True for simplicity on Windows,
    # but REPEAT: be cautious with untrusted inputs.
    shell = True
    # If you wanted to avoid shell=True for better security with complex inputs,
    # you'd parse the command string into a list of arguments and pass shell=False.
    # command_and_args = command.split() # Basic example, needs robust parsing

    try:
        # Use subprocess.run to execute the command
        # capture_output=True captures stdout and stderr
        # text=True decodes stdout/stderr as text
        # timeout prevents the command from running forever
        # check=True would raise CalledProcessError for non-zero exit codes,
        # but we'll check returncode manually to report output even on error.
        result = subprocess.run(
            command,
            shell=shell,
            capture_output=True,
            text=True,
            timeout=60, # Timeout after 60 seconds
            # executable='/bin/bash' if sys.platform != 'win32' else 'cmd.exe' # Specify shell explicitly
        )

        output = f"Command Output:\n---\n{result.stdout}\n---\n"
        if result.stderr:
            output += f"Command Error:\n---\n{result.stderr}\n---\n"

        if result.returncode != 0:
            output += f"Command failed with exit code: {result.returncode}\n"

        return output

    except FileNotFoundError:
         return f"Error: Command not found. Make sure the command '{command.split()[0]}' is in your system's PATH."
    except subprocess.TimeoutExpired:
         return f"Error: Command timed out after 60 seconds."
    except Exception as e:
        return f"An unexpected error occurred while running command: {e}"

@tool
def search_tavily(query: str) -> str:
    """Searches TaviLy for the given query.
    Returns the search results or an error message."""
    try:
        # Perform the search using the TavilySearchResults tool
        search_results = TavilySearchResults(query=query)
        results = search_results.run()
        if results:
            return f"Search results for '{query}':\n---\n{results}\n---"
        else:
            return f"No results found for '{query}'."
    except Exception as e:
        return f"Error searching TaviLy: {e}"
    
@tool
def rename_file(current_path: str, new_path: str) -> str:
    """Renames a file from current_path to new_path.
    Returns a success message or an error if the file cannot be renamed."""
    try:
        # Basic check to ensure the file exists before attempting to rename
        if not os.path.exists(current_path):
             return f"Error: File not found at '{current_path}'. Cannot rename."
        if not os.path.isfile(current_path):
             return f"Error: Path '{current_path}' is not a file. Cannot rename."

        os.rename(current_path, new_path)
        return f"File successfully renamed from '{current_path}' to '{new_path}'."
    except FileExistsError:
        return f"Error: A file already exists at the destination '{new_path}'. Cannot rename."
    except FileNotFoundError:
         # This might happen if the new_path includes non-existent directories
         return f"Error: The destination path '{new_path}' contains a directory that does not exist."
    except Exception as e:
        return f"An unexpected error occurred while renaming file from '{current_path}' to '{new_path}': {e}"

search_tool = TavilySearchResults(max_results=5)

plogf.write(now() +"\t Defined Tools...\n")
#------------------------------------------------------------------------------------------------------
plogf.write(now() +"\t Step 6: Creating Tool List...\n")
# Create a list of all tools available to the agent
tools = [read_files_folders, edit_files_folders, create_files_folders, run_terminal_command, rename_file, search_tool]
plogf.write(now() +"\t Created Tool List...\n")
#------------------------------------------------------------------------------------------------------
plogf.write(now() +"\t Step 7: Creating Agent...\n")
# This combines the LLM, the list of tools, and the prompt template
agent = create_tool_calling_agent(llm, tools, prompt)
plogf.write(now() +"\t Created Agent...\n")
#------------------------------------------------------------------------------------------------------
plogf.write(now() +"\t Step 8: Creating Agent Executor...\n")
# Create the Agent Executor
# This is the runtime that takes the agent's decisions and executes tool calls
# setting verbose=True helps see the agent's thought process and tool usage
agent_executor = AgentExecutor(agent=agent, tools=tools, verbose=True)
plogf.write(now() +"\t Created Agent Executor...\n")
#------------------------------------------------------------------------------------------------------
plogf.write(now() +"\t Step 9: Starting CLI Interaction...\n")
# --- CLI Interaction ---
print("CLI Agent with Tools (Gemini 2.0 Flash) is ready.")
print("Type your message and press Enter. Type 'quit' to exit.")
plogf.write(now() +"\t Started CLI Interaction...\n")
#------------------------------------------------------------------------------------------------------
plogf.write(now() +"\t Step 10: Initializing chat history...\n")
# In-memory storage for chat history for the current session
chat_history = []
plogf.write(now() +"\t Initialized chat history...\n")
#------------------------------------------------------------------------------------------------------
plogf.write(now() +"\t Step 11: Starting CLI chat loop...\n")
# CLI chat loop
while True:
    user_input = input("You: ")
    hlogf.write(now() +"\t User input: " + user_input + "\n")
    plogf.write(now() +"\t User input\n")
    if user_input.lower() == 'quit' or user_input.lower() == 'exit' or user_input.lower() == 'q' or user_input.lower() == 'e' or user_input.lower() == 'bye': 
        plogf.write(now() +"\t User requested to quit. Exiting...\n")
        break

    if not user_input.strip():
        plogf.write(now() +"\t User input is empty. Continuing to next iteration.\n")
        continue

    try:
        # Invoke the agent executor with the current input and chat history
        # The agent_executor handles deciding whether to call the LLM or a tool
        result = agent_executor.invoke({"input": user_input, "chat_history": chat_history})

        # The final output is in the 'output' key of the result dictionary
        ai_response = result['output']
        print(f"Agent: {ai_response}")
        hlogf.write(now() +"\t Agent response: " + ai_response + "\n")
        plogf.write(now() +"\t Agent response\n")

        # Append messages to chat history
        # Note: The AgentExecutor internally manages the tool call messages in the scratchpad,
        # but for simple chat history we just add the user input and final AI output.
        chat_history.append(HumanMessage(content=user_input))
        chat_history.append(AIMessage(content=ai_response))

    except Exception as e:
        plogf.write(now() +"\t Error during interaction: " + str(e) + "\n")
        print(f"Error during interaction: {e}")
        # Handle errors, maybe add an AI message indicating an error occurred
plogf.write(now() +"\t Step 12: Ending all and exiting CLI Interaction...\n")
plogf.close()
hlogf.close()
print("\nAgent session ended.")