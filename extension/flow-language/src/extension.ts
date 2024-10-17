import { ExtensionContext, window, languages } from 'vscode';
import FlowFormatter from './formatter';

export function activate(context: ExtensionContext) {
    const console = window.createOutputChannel('Flow Formatter');
    const formatter = new FlowFormatter(console);

    context.subscriptions.push(
        languages.registerDocumentFormattingEditProvider('flow', formatter)
    );
}

export function deactivate() {}
