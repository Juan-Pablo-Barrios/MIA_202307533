import { useState } from "react";
import Header from "../components/Header";
import Toolbar from "../components/Toolbar";
import CommandInput from "../components/CommandInput";
import CommandOutput from "../components/CommandOutput";
import { executeCommands } from "../services/api";

export default function Home() {
  const [inputText, setInputText] = useState(
    `# Ejemplo de comandos
mkdisk -size=10 -path="/home/Disco1.mia"
fdisk -size=300 -path="/home/Disco1.mia" -name=Part1
mount -path="/home/Disco1.mia" -name=Part1`
  );

  const [outputText, setOutputText] = useState(
    `[INFO] Aquí aparecerán los resultados enviados por el backend...`
  );

  const [fileName, setFileName] = useState("");
  const [loading, setLoading] = useState(false);

  const handleFileChange = (e) => {
    const file = e.target.files[0];
    if (!file) return;

    setFileName(file.name);

    const reader = new FileReader();
    reader.onload = (event) => {
      setInputText(event.target.result);
    };
    reader.readAsText(file);
  };

  const handleExecute = async () => {
    if (!inputText.trim()) {
      setOutputText("Error: no hay comandos para ejecutar.");
      return;
    }

    setLoading(true);
    setOutputText("Ejecutando comandos...");

    const response = await executeCommands(inputText);
    setOutputText(response.output);

    setLoading(false);
  };

  const handleClear = () => {
    setInputText("");
    setOutputText("");
    setFileName("");
  };

  return (
    <div className="min-h-screen bg-gray-100 p-6 md:p-10">
      <div className="mx-auto max-w-6xl rounded-3xl bg-white shadow-xl border border-gray-200 overflow-hidden">
        <Header />

        <div className="p-6 md:p-8">
          <Toolbar
            fileName={fileName}
            onFileChange={handleFileChange}
            onExecute={handleExecute}
            onClear={handleClear}
            loading={loading}
          />

          <div className="grid gap-6">
            <CommandInput value={inputText} onChange={setInputText} />
            <CommandOutput value={outputText} />
          </div>
        </div>
      </div>
    </div>
  );
}