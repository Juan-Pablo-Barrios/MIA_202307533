export default function CommandOutput({ value }) {
  return (
    <div>
      <h2 className="mb-2 text-lg font-semibold text-gray-900">Salida</h2>
      <textarea
        value={value}
        readOnly
        className="h-72 w-full resize-none rounded-2xl border border-gray-800 bg-[#111827] p-4 font-mono text-sm text-white outline-none"
        placeholder="Aquí aparecerán los resultados..."
      />
    </div>
  );
}