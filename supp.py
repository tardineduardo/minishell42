import re
import sys

def replace_valgrind_blocks_inplace(filepath):
    """
    Reads a text file and deletes Valgrind memory error blocks 
    that contain 'readline'. All other blocks remain unchanged.
    
    Args:
        filepath (str): Path to the text file.
    """
    try:
        with open(filepath, 'r') as f:
            content = f.read()
        
        # Pattern that only matches blocks containing 'readline'
        readline_pattern = r"(^|\n)(==\d+==\s*\n==\d+==(?:(?!\n==\d+==\s*(?:\n|$)).)*?libreadline\.so\.8\.0(?:(?!\n==\d+==\s*(?:\n|$)).)*?)(?=\n==\d+==\s*(?:\n|$))"
        
        def replace_func(match):
            # If match started with a newline, preserve it
            if match.group(1) == '\n':
                return '\n'
            # If at start of file, return empty string
            return ''
        
        # Delete readline blocks while preserving necessary newlines
        cleaned_content = re.sub(readline_pattern, replace_func, content, flags=re.MULTILINE|re.DOTALL)
        
        # Remove sequential empty lines
        cleaned_content = re.sub(r'\n\s*\n+', '\n', cleaned_content)
        
        with open(filepath, 'w') as f:
            f.write(cleaned_content)
        
        #print(f"Readline blocks removed and sequential empty lines cleaned in: {filepath}")
    
    except FileNotFoundError:
        print(f"Error: File '{filepath}' not found.")
    except Exception as e:
        print(f"An error occurred: {e}")

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python clean_valgrind.py <filename>")
        sys.exit(1)
    
    filename = sys.argv[1]
    replace_valgrind_blocks_inplace(filename)