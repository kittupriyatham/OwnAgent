import os
import logging
from langchain_google_genai import ChatGoogleGenerativeAI
from langchain_core.messages import HumanMessage

# Configure basic logging
logging.basicConfig(filename='app.log', level=logging.INFO,
                    format='%(asctime)s - %(levelname)s - %(message)s')

# Optional: Create a separate logger for chat events
chat_logger = logging.getLogger('chat_events')
chat_logger.setLevel(logging.INFO)
chat_handler = logging.FileHandler('chat.log')
chat_formatter = logging.Formatter('%(asctime)s - %(message)s')
chat_handler.setFormatter(chat_formatter)
chat_logger.addHandler(chat_handler)

logging.info("Script started")

print("Step 1: Verifying Python Environment and Gemini API Key...")
logging.info("Verifying Python Environment and Gemini API Key...")

# Check if the GEMINI_API_KEY environment variable is set
gemini_api_key = os.getenv("GEMINI_API_KEY")
if not gemini_api_key:
    print("FAIL: GEMINI_API_KEY environment variable is NOT set.")
    logging.error("GEMINI_API_KEY environment variable is NOT set.")
    print("Please ensure you have set the GEMINI_API_KEY environment variable correctly.")
    print("For Windows Command Prompt: setx GEMINI_API_KEY \"YOUR_API_KEY\"")
    print("For Windows PowerShell: $env:GEMINI_API_KEY=\"YOUR_API_KEY\"")
    print("Remember to replace YOUR_API_KEY with your actual key and restart your terminal.")
    exit()
else:
    print("SUCCESS: GEMINI_API_KEY environment variable is set.")
    logging.info("GEMINI_API_KEY environment variable is set.")

# Attempt to load the Gemini model using the API key
try:
    llm = ChatGoogleGenerativeAI(model="gemini-2.0-flash")
    print("SUCCESS: Successfully loaded Gemini model using the API key.")
    logging.info("Successfully loaded Gemini model using the API key.")
except Exception as e:
    print(f"FAIL: Could not load Gemini model. Error: {e}")
    logging.error(f"Could not load Gemini model. Error: {e}")
    print("Please check if your API key is correct and has access to the 'gemini-2.0-flash' model.")
    print("Also ensure you have a stable internet connection.")
    exit()

# If both checks pass, the basic setup is good
print("\nStep 1 Verification Complete.")
logging.info("Step 1 Verification Complete.")
print("Your Python environment and Gemini API connection appear to be working.")
logging.info("Your Python environment and Gemini API connection appear to be working.")

# You can optionally add a simple test call here if you like,
# but the main goal of this step is just setup verification.
try:
    message = HumanMessage(content="Test connection.")
    response = llm.invoke([message])
    print(f"Test call response snippet: {str(response.content)[:50]}...")
    logging.info(f"Test call response snippet: {str(response.content)[:50]}...")
    chat_logger.info(f"Test call query: Test connection.  Response: {str(response.content)[:50]}...") # Log the chat event
except Exception as e:
     print(f"Warning: Test call failed. Error: {e}")
     logging.warning(f"Test call failed. Error: {e}")

logging.info("Script finished")