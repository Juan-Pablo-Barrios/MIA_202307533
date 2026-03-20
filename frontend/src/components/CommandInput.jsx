export default function CommandInput({ value, onChange }) {
  return (
    <div>
      <h2 className="mb-2 text-lg font-semibold text-gray-900">Entrada</h2>
      <textarea
        value={value}
        onChange={(e) => onChange(e.target.value)}
        className="h-72 w-full resize-none rounded-2xl border border-gray-800 bg-[#111827] p-4 font-mono text-sm text-green-200 outline-none focus:ring-2 focus:ring-blue-500"
        placeholder="Escribe aquí los comandos o carga un archivo .smia"
      />
    </div>
  );
}